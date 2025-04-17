#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>

#define LOGFILE "debugmon.log"
#define PIDFILE_FMT "debugmon_%s.pid"
#define LOCKFILE_FMT "fail_%s.lock"
#define BUFSIZE 512

uid_t get_uid(const char *username) {
    struct passwd *pw = getpwnam(username);
    if (!pw) {
        fprintf(stderr, "User %s not found.\n", username);
        exit(1);
    }
    return pw->pw_uid;
}

void tulisLog(const char *procName, const char *status) {
    FILE *log = fopen(LOGFILE, "a");
    if (!log) {
        perror("Failed to open log file");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(log, "[%02d:%02d:%04d]-[%02d:%02d:%02d]_%s_%s\n",
        t->tm_mday, t->tm_mon+1, t->tm_year+1900,
        t->tm_hour, t->tm_min, t->tm_sec,
        procName, status);
    fclose(log);
}

int isUserBlocked(const char *user) {
    char lockpath[BUFSIZE];
    snprintf(lockpath, sizeof(lockpath), LOCKFILE_FMT, user);
    return access(lockpath, F_OK) == 0;
}

void listProcess(const char *user) {
    uid_t target_uid = get_uid(user);
    if (target_uid == (uid_t)-1) return;

    DIR *proc = opendir("/proc");
    if (!proc) {
        perror("Failed to open /proc");
        return;
    }

    FILE *meminfo = fopen("/proc/meminfo", "r");
    unsigned long memTotal = 0;
    char line[BUFSIZE];
    if (meminfo) {
        while (fgets(line, sizeof(line), meminfo)) {
            if (sscanf(line, "MemTotal: %lu kB", &memTotal) == 1)
                break;
        }
        fclose(meminfo);
    }

    FILE *statfile = fopen("/proc/stat", "r");
    unsigned long long totalCpuTime = 0;
    if (statfile) {
        fgets(line, sizeof(line), statfile);
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
        sscanf(line, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu",
               &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
        totalCpuTime = user + nice + system + idle + iowait + irq + softirq + steal;
        fclose(statfile);
    }

    printf("==================================================================\n");
    printf("Process list for user %s:\n", user);
    printf("==================================================================\n");
    printf("PID\tCOMMAND\t\tCPU(s)\tMEM(KB)\tCPU(%%)\tMEM(%%)\n");
    printf("------------------------------------------------------------------\n");
   
    struct dirent *ent;
    while ((ent = readdir(proc)) != NULL) {
        if (!isdigit(ent->d_name[0])) continue;

        char statuspath[BUFSIZE];
        snprintf(statuspath, BUFSIZE, "/proc/%s/status", ent->d_name);

        FILE *f = fopen(statuspath, "r");
        if (!f) continue;

        char line[BUFSIZE];
        uid_t uid = -1;
        char name[64] = "";
        unsigned long mem = 0;

        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "Uid:", 4) == 0)
                sscanf(line, "Uid:\t%d", &uid);
            else if (strncmp(line, "Name:", 5) == 0)
                sscanf(line, "Name:\t%63s", name);
            else if (strncmp(line, "VmRSS:", 6) == 0)
                sscanf(line, "VmRSS:\t%lu", &mem);
        }
        fclose(f);

        if (uid == target_uid) {
            char statpath[BUFSIZE];
            snprintf(statpath, BUFSIZE, "/proc/%s/stat", ent->d_name);

            FILE *statfile = fopen(statpath, "r");
            unsigned long utime = 0, stime = 0;
            if (statfile) {
                int dummy;
                char comm[256], state;
                fscanf(statfile, "%d %s %c", &dummy, comm, &state);
                for (int i = 0; i < 11; i++) fscanf(statfile, "%*s");
                fscanf(statfile, "%lu %lu", &utime, &stime);
                fclose(statfile);
            }

            double cpu_time = (double)(utime + stime) / sysconf(_SC_CLK_TCK);

            double cpu_usage = 0.0;
            if (totalCpuTime > 0) {
                cpu_usage = 100.0 * (utime + stime) / totalCpuTime;
            }

            double mem_usage = 0.0;
            if (memTotal > 0) {
                mem_usage = 100.0 * mem / memTotal;
            }

            printf("%s\t%-15s\t%.1f\t%lu\t%.2f\t%.2f\n",
                ent->d_name, name, cpu_time, mem, cpu_usage, mem_usage);
            tulisLog(name, "RUNNING");
        }
    }
    closedir(proc);
}

void daemonUser(const char *user) {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) {
        char pidfile[BUFSIZE];
        snprintf(pidfile, sizeof(pidfile), PIDFILE_FMT, user);
        FILE *f = fopen(pidfile, "w");
        if (f) {
            fprintf(f, "%d\n", pid);
            fclose(f);
        }
        exit(EXIT_SUCCESS);
    }

    umask(0);
    setsid();
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    uid_t target_uid = get_uid(user);

    while (1) {
        if (!isUserBlocked(user)) {
            DIR *proc = opendir("/proc");
            struct dirent *ent;
            while ((ent = readdir(proc)) != NULL) {
                if (!isdigit(ent->d_name[0])) continue;

                char statuspath[BUFSIZE];
                snprintf(statuspath, BUFSIZE, "/proc/%s/status", ent->d_name);

                FILE *f = fopen(statuspath, "r");
                if (!f) continue;

                char line[BUFSIZE];
                uid_t uid = -1;
                char name[64] = "";
                while (fgets(line, sizeof(line), f)) {
                    if (strncmp(line, "Uid:", 4) == 0)
                        sscanf(line, "Uid:\t%d", &uid);
                    else if (strncmp(line, "Name:", 5) == 0)
                        sscanf(line, "Name:\t%63s", name);
                }
                fclose(f);

                if (uid == target_uid) {
                    tulisLog(name, "RUNNING");
                }
            }
            closedir(proc);
        }
        sleep(5);
    }
}

void stopProcess(const char *user) {
    char pidfile[BUFSIZE];
    snprintf(pidfile, sizeof(pidfile), PIDFILE_FMT, user);

    FILE *f = fopen(pidfile, "r");
    if (!f) {
        printf("No active daemon found for user %s.\n", user);
        return;
    }

    pid_t pid;
    fscanf(f, "%d", &pid);
    fclose(f);

    if (kill(pid, SIGTERM) == 0) {
        remove(pidfile);
        tulisLog("debugmon_stop", "RUNNING");
        printf("Monitoring stopped.\n");
    } else {
        perror("Failed to stop daemon");
    }
}

void failProcess(const char *user) {
    uid_t target_uid = get_uid(user);
    if (target_uid == (uid_t)-1) return;

    char lockpath[BUFSIZE];
    snprintf(lockpath, sizeof(lockpath), LOCKFILE_FMT, user);
    FILE *lock = fopen(lockpath, "w");
    if (lock) fclose(lock);

    DIR *proc = opendir("/proc");
    if (!proc) {
        perror("Failed to open /proc");
        return;
    }

    struct dirent *ent;
    while ((ent = readdir(proc)) != NULL) {
        if (!isdigit(ent->d_name[0])) continue;

        char statuspath[BUFSIZE];
        snprintf(statuspath, BUFSIZE, "/proc/%s/status", ent->d_name);

        FILE *f = fopen(statuspath, "r");
        if (!f) continue;

        char line[BUFSIZE];
        uid_t uid = -1;
        char name[64] = "";
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "Uid:", 4) == 0)
                sscanf(line, "Uid:\t%d", &uid);
            else if (strncmp(line, "Name:", 5) == 0)
                sscanf(line, "Name:\t%63s", name);
        }
        fclose(f);

        if (uid == target_uid &&
            strcmp(name, "debugmon") != 0 &&
            strcmp(name, "bash") != 0 &&
            strcmp(name, "zsh") != 0 &&
            strcmp(name, "gnome-terminal") != 0 &&
            strcmp(name, "systemd") != 0) {
            kill(atoi(ent->d_name), SIGKILL);
            tulisLog(name, "FAILED");
        }
    }
    closedir(proc);
    printf("All processes for user %s have been terminated and blocked.\n", user);
}

void revertProcess(const char *user) {
    char lockfile[BUFSIZE];
    snprintf(lockfile, sizeof(lockfile), LOCKFILE_FMT, user);
    if (remove(lockfile) == 0) {
        tulisLog("debugmon_revert", "RUNNING");
        printf("User %s is now allowed to run processes normally.\n", user);
    } else {
        printf("No restrictions found for user %s.\n", user);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <list|daemon|stop|fail|revert> <user>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "list") == 0) {
        listProcess(argv[2]);
    } else if (strcmp(argv[1], "daemon") == 0) {
        daemonUser(argv[2]);
    } else if (strcmp(argv[1], "stop") == 0) {
        stopProcess(argv[2]);
    } else if (strcmp(argv[1], "fail") == 0) {
        failProcess(argv[2]);
    } else if (strcmp(argv[1], "revert") == 0) {
        revertProcess(argv[2]);
    } else {
        printf("Unknown command.\n");
        return 1;
    }

    return 0;
}
