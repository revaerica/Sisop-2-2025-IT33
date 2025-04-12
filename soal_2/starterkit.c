#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_PATH 512
#define MAX_FILENAME 256
#define STARTER_ZIP "starter_kit.zip"
#define STARTER_DIR "starter_kit"
#define QUARANTINE_DIR "quarantine"
#define LOG_FILE "activity.log"

void log_activity(const char *message) {
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "[%d-%m-%Y][%H:%M:%S]", t);

    char logbuf[512];
    snprintf(logbuf, sizeof(logbuf), "%s - %s\n", timebuf, message);
    fputs(logbuf, log);
    fclose(log);
}

void ensure_directories() {
    mkdir(QUARANTINE_DIR, 0755);
}

int folder_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

void download_and_unzip() {
    if (folder_exists(STARTER_DIR)) return;

    printf("Mendownload starter kit...\n");
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"/bin/sh", "-c",
            "wget --show-progress --no-check-certificate \"https://drive.google.com/uc?export=download&id=1_5GxIGfQr3mNKuavJbte_AoRkEQLXSKS\" -O starter_kit.zip",
            NULL};
        execv("/bin/sh", args);
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (!file_exists(STARTER_ZIP)) {
            fprintf(stderr, "Gagal mendownload starter kit.\n");
            return;
        }
    }

    printf("Unzipping starter kit...\n");
    pid = fork();
    if (pid == 0) {
        char *args[] = {"/bin/sh", "-c", "unzip -q starter_kit.zip -d starter_kit", NULL};
        execv("/bin/sh", args);
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }

    remove(STARTER_ZIP);
}

int is_valid_base64(const char *str) {
    size_t len = strlen(str);
    if (len % 4 != 0) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum(str[i]) && str[i] != '+' && str[i] != '/' && str[i] != '=') {
            return 0;
        }
    }
    return 1;
}

char *base64_decode(const char *src) {
    int pipe_in[2], pipe_out[2];
    pipe(pipe_in);
    pipe(pipe_out);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(pipe_in[0], STDIN_FILENO);
        dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_in[1]);
        close(pipe_out[0]);
        execlp("base64", "base64", "-d", NULL);
        exit(EXIT_FAILURE);
    }

    close(pipe_in[0]);
    close(pipe_out[1]);
    write(pipe_in[1], src, strlen(src));
    close(pipe_in[1]);

    char *decoded = calloc(MAX_FILENAME, 1);
    read(pipe_out[0], decoded, MAX_FILENAME - 1);
    close(pipe_out[0]);

    waitpid(pid, NULL, 0);
    return decoded;
}

void daemon_decryptor() {
    pid_t pid = fork();
    if (pid > 0) return;

    if (pid == 0) {
        setsid();

        char logmsg[512];
        snprintf(logmsg, sizeof(logmsg), "Successfully started decryption process with PID %d.", getpid());
        log_activity(logmsg);

        while (1) {
            DIR *d = opendir(STARTER_DIR);
            if (!d) continue;

            struct dirent *f;
            while ((f = readdir(d))) {
                if (f->d_type == DT_REG && is_valid_base64(f->d_name)) {
                    // tidak mencatat aktivitas
                }
            }

            closedir(d);
            sleep(5);
        }
    }
}

void move_files(const char *from, const char *to, const char *log_format) {
    DIR *d = opendir(from);
    if (!d) return;

    struct dirent *f;
    while ((f = readdir(d))) {
        if (f->d_type == DT_REG) {
            char src[MAX_PATH], dest[MAX_PATH], logmsg[512];
            char filename[MAX_FILENAME];
            strncpy(filename, f->d_name, MAX_FILENAME - 1);
            filename[MAX_FILENAME - 1] = '\0';

            if (is_valid_base64(filename)) {
                char *decoded = base64_decode(filename);
                if (decoded) {
                    snprintf(dest, MAX_PATH, "%s/%s", to, decoded);
                    free(decoded);
                } else {
                    snprintf(dest, MAX_PATH, "%s/%s", to, filename);
                }
            } else {
                snprintf(dest, MAX_PATH, "%s/%s", to, filename);
            }

            snprintf(src, MAX_PATH, "%s/%s", from, filename);
            rename(src, dest);

            snprintf(logmsg, sizeof(logmsg), log_format, filename);
            log_activity(logmsg);
        }
    }

    closedir(d);
}

void delete_quarantine_files() {
    DIR *d = opendir(QUARANTINE_DIR);
    if (!d) return;

    struct dirent *f;
    while ((f = readdir(d))) {
        if (f->d_type == DT_REG) {
            char path[MAX_PATH], logmsg[512];
            snprintf(path, MAX_PATH, "%s/%s", QUARANTINE_DIR, f->d_name);
            remove(path);
            snprintf(logmsg, sizeof(logmsg), "%s - Successfully deleted.", f->d_name);
            log_activity(logmsg);
        }
    }

    closedir(d);
}

void shutdown_decryptor() {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"/bin/sh", "-c", "pkill -f 'starterkit --decrypt'", NULL};
        execv("/bin/sh", args);
        exit(1);
    } else {
        wait(NULL);
        char logmsg[512];
        snprintf(logmsg, sizeof(logmsg), "Successfully shut off decryption process with PID %d.", getpid());
        log_activity(logmsg);
    }
}

int main(int argc, char *argv[]) {
    ensure_directories();
    download_and_unzip();

    if (argc == 1) return 0;

    if (strcmp(argv[1], "--decrypt") == 0) {
        daemon_decryptor();
    } else if (strcmp(argv[1], "--quarantine") == 0) {
        move_files(STARTER_DIR, QUARANTINE_DIR, "%s - Successfully moved to quarantine directory.");
    } else if (strcmp(argv[1], "--return") == 0) {
        move_files(QUARANTINE_DIR, STARTER_DIR, "%s - Successfully returned to starter kit directory.");
    } else if (strcmp(argv[1], "--eradicate") == 0) {
        delete_quarantine_files();
    } else if (strcmp(argv[1], "--shutdown") == 0) {
        shutdown_decryptor();
    } else {
        printf("Argumen tidak dikenal. Gunakan salah satu dari: --decrypt, --quarantine, --return, --eradicate, --shutdown\n");
    }

    return 0;
}