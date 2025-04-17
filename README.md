# Sisop-2-2025-IT33

#### Nama Anggota
1. Ni'mah Fauziyyah Atok (5027241103)
2. Revalina Erica Permatasari (5027241007)

## Daftar Isi
1. [Soal 1](#soal-1)
2. [Soal 2](#soal-2)
4. [Soal 4](#soal-4)

## Soal 1
### Oleh: Ni'mah Fauziyyah A

### Soal 
Kamu terbangun secara tiba-tiba di suatu lokasi (https://dragon-pw-checker.vercel.app/) yang tidak diketahui, saat kamu melihat sekitar yang terlihat hanyalah kegelapan dan sebuah pintu dengan dua mata berwarna yang melihatmu dari lubang pintu tersebut. <br>
Ia merupakan naga bernama Cyrus yang menjaga pintu tersebut dan kamu harus meng-input password yang benar untuk masuk. Karena tidak mungkin untuk menebak password, Ia memberikanmu sebuah clue Clues.zip (https://drive.google.com/file/d/1xFn1OBJUuSdnApDseEczKhtNzyGekauK/view). Untungnya, kamu merupakan Directory Lister yang jago sehingga clue ini dapat berubah menjadi sebuah password.

### Jawaban

#### A. Downloading the Clues
Karena kamu telah diberikan sebuah link Clues oleh Cyrus, kamu membuat file bernama action.c 
yang dimana kalau dijalankan seperti biasa tanpa argumen tambahan akan mendownload dan meng-unzip file tersebut secara langsung. 
Saat kamu melihat isi dari Clues tersebut, isinya berupa 4 folder yakni ClueA - ClueD 
dan di masing-masing folder tersebut terdapat .txt files dan isinya masih tidak jelas, 
mungkin beberapa kata di dalam .txt file yang dapat dicari di inventory website? 
(Note: inventory bersifat untuk seru-seruan saja).  
Jangan lupa untuk menghapus Clues.zip setelah diekstrak 
dan buatlah apabila folder Clues sudah ada, maka command tersebut tidak akan mendownload Clues.zip lagi apabila dijalankan.

##### Kode
```
void download_and_unzip() {
    if (access("Clues", F_OK) == 0) {
        printf("Folder 'Clues' sudah ada.\n");
        return;
    }

    char *wget_args[] = {
        "wget", "-O", "Clues.zip",
        "https://drive.usercontent.google.com/download?id=1xFn1OBJUuSdnApDseEczKhtNzyGekauK", NULL
    };
    run_command("wget", wget_args);

    char *unzip_args[] = {"unzip", "Clues.zip", NULL};
    run_command("unzip", unzip_args);

    remove("Clues.zip");
    printf("Clues.zip berhasil didownload dan diekstrak.\n");
}
```

##### Penjelasan
- Mengecek apakah folder `Clues` sudah ada.
- Jika belum, file `Clues.zip` akan didownload menggunakan `wget`.
- File kemudian diekstrak menggunakan `unzip`, dan file zip akan dihapus.
- Fungsi `run_command()` digunakan untuk menjalankan perintah eksternal melalui `fork()` dan `execvp()`.

---

#### B. Filtering the Files
Karena kebanyakan dari file tersebut berawal dengan 1 huruf atau angka, kamu pun mencoba untuk memindahkan file-file 
yang hanya dinamakan dengan 1 huruf dan 1 angka tanpa special character kedalam folder bernama Filtered. 
Kamu tidak suka kalau terdapat banyak clue yang tidak berguna jadi disaat melakukan filtering, file yang tidak terfilter dihapus. 
Karena kamu tidak ingin membuat file kode lagi untuk filtering, maka kamu menggunakan file sebelumnya untuk filtering file-file tadi 
dengan menambahkan argumen saat ingin menjalankan action.c

##### Kode
```
void filter_files() {
    mkdir("Filtered", 0777);
    char *folders[] = {"Clues/ClueA", "Clues/ClueB", "Clues/ClueC", "Clues/ClueD"};

    for (int i = 0; i < 4; i++) {
        DIR *dir = opendir(folders[i]);
        ...
        while ((entry = readdir(dir)) != NULL) {
            ...
            if (strlen(name) == 5 && isalnum(name[0]) &&
                strcmp(&name[1], ".txt") == 0) {
                ...
                rename(source, target);
            } else {
                remove(source);
            }
        }
        closedir(dir);
    }

    printf("File berhasil difilter ke folder 'Filtered'.\n");
}
```

##### Penjelasan
- Membuka folder clue satu per satu.
- Mengecek nama file, hanya file dengan panjang 5 karakter dan diawali huruf/angka + `.txt` yang valid.
- File valid dipindahkan ke folder `Filtered`, sisanya dihapus.

---

#### C. Combine the File Content
Di setiap file .txt yang telah difilter terdapat satu huruf dan agar terdapat progress, 
Cyrus memberikan clue tambahan untuk meletakan/redirect isi dari setiap .txt file tersebut kedalam satu file yaitu Combined.txt dengan menggunakan FILE pointer. 
Tetapi, terdapat urutan khusus saat redirect isi dari .txt tersebut, yaitu urutannya bergantian dari .txt dengan nama angka lalu huruf lalu angka lagi lalu huruf lagi. 
Lalu semua file .txt sebelumnya dihapus. Seperti halnya tadi, agar efisien kamu ingin menjalankan action.c dengan argumen tambahan.

##### Kode
```
void combine_files() {
    FILE *out = fopen("Combined.txt", "w");
    ...
    for (int i = 0; i < 6; i++) {
        char *pair[] = {nums[i], alphas[i]};
        for (int j = 0; j < 2; j++) {
            ...
            while ((ch = fgetc(in)) != EOF) {
                fputc(ch, out);
            }
            fclose(in);
            remove(path);
        }
    }
    fclose(out);
    printf("Isi file berhasil digabung ke 'Combined.txt'.\n");
}
```

##### Penjelasan
- Membaca isi file dari `Filtered/1.txt` hingga `Filtered/6.txt` dan `a.txt` hingga `f.txt`.
- Digabungkan menjadi satu file `Combined.txt`.
- Setelah digabung, file aslinya dihapus.

---

#### D. Decode the file 
Karena isi Combined.txt merupakan string yang random, kamu memiliki ide untuk menggunakan Rot13 untuk decode string tersebut 
dan meletakan hasil dari yang telah di-decode tadi kedalam file bernama Decoded.txt. Jalankan file action.c dengan argumen tambahan 
untuk proses decoding ini. 

##### Kode
```c
char rot13(char c) {
    if ('a' <= c && c <= 'z') return ((c - 'a' + 13) % 26) + 'a';
    if ('A' <= c && c <= 'Z') return ((c - 'A' + 13) % 26) + 'A';
    return c;
}

void decode_file() {
    FILE *in = fopen("Combined.txt", "r");
    FILE *out = fopen("Decoded.txt", "w");
    ...
    while ((ch = fgetc(in)) != EOF) {
        fputc(rot13(ch), out);
    }
    fclose(in);
    fclose(out);
    printf("Isi file berhasil didekode ke 'Decoded.txt'.\n");
}
```

##### Penjelasan
- ROT13 adalah metode substitusi huruf, menggantikan huruf dengan huruf ke-13 dari abjad.
- Proses membaca `Combined.txt`, mengenkripsi huruf dengan `rot13()`, lalu menyimpannya ke `Decoded.txt`.

---

#### E. Struktur Program dan Cara Pemakaian  
Program utama dapat dijalankan dengan beberapa mode.

##### Kode
```c
int main(int argc, char *argv[]) {
    if (argc == 1) {
        download_and_unzip();
    } else if (argc == 3 && strcmp(argv[1], "-m") == 0) {
        if (strcmp(argv[2], "Filter") == 0) filter_files();
        else if (strcmp(argv[2], "Combine") == 0) combine_files();
        else if (strcmp(argv[2], "Decode") == 0) decode_file();
        else print_usage();
    } else {
        print_usage();
    }
    return 0;
}
```

##### Penjelasan
- Jika program dijalankan tanpa argumen → download & ekstrak Clues.zip.
- Jika menggunakan `-m`, maka program menjalankan mode spesifik:
  - `Filter`: filter file
  - `Combine`: gabungkan file
  - `Decode`: dekode isi file


#### F. Password Check
Karena kamu sudah mendapatkan password tersebut, kamu mencoba untuk mengecek apakah password yang sudah kamu dapatkan itu benar atau tidak 
dengan cara di-input ke lokasi (https://dragon-pw-checker.vercel.app/) tadi.

---
#### G. Kesimpulan  
Program ini membantu "Observer" dalam menganalisis clues yang tersebar, memastikan hanya fragment yang valid diproses, dan menerjemahkan pesan tersembunyi dari Seeker menggunakan ROT13. Dengan modularitas fungsi, program lebih terstruktur dan mudah dikembangkan untuk kebutuhan misi selanjutnya.

## Soal 2
### Oleh: Ni'mah Fauziyyah A

### Soal
Kanade Yoisaki, seorang siswi SMA yang nolep dan composer handal,mengalami masalah pada komputernya yang terkena malware ringan. Bersama dengan Mizuki, Mafuyu, dan Ena, mereka mencoba membangun sistem starter kit pendeteksi virus. Dari soal ini diminta membantu dengan membangun program `starterkit` yang mampu: <br>
- Mendownload dan unzip starter kit. <br>
- Menjalankan proses dekripsi nama file terenkripsi base64. <br>
- Memindahkan file antara starter kit dan karantina. <br>
- Menghapus file dalam karantina. <br>
- Mematikan daemon dekripsi. <br>
- Menangani error dari argumen. <br>
- Mencatat semua aktivitas ke dalam activity.log. <br>

### Jawaban

#### A. Download Starter Kit
Sebagai teman yang baik, Mafuyu merekomendasikan Kanade untuk mendownload dan unzip sebuah starter kit berisi file - file acak(sudah termasuk virus) melalui link berikut `(https://drive.google.com/file/d/1_5GxIGfQr3mNKuavJbte_AoRkEQLXSKS/view)` agar dapat membantu Kanade dalam mengidentifikasi virus - virus yang akan datang. Jangan lupa untuk menghapus file zip asli setelah melakukan unzip.

##### Kode
```
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
```

##### Penjelasan
- Mengecek apakah folder sudah ada.
- Jika belum, akan download dari Google Drive dan unzip ke `starter_kit`.

---

#### B. Menjalankan Daemon Dekripsi
Setelah mendownload starter kit tersebut, Mafuyu ternyata lupa bahwa pada starter kit tersebut, tidak ada alat untuk mendecrypt nama dari file yang diencrypt menggunakan algoritma Base64. Oleh karena itu, bantulah Mafuyu untuk membuat sebuah directory karantina yang dapat mendecrypt nama file yang ada di dalamnya (Hint: gunakan daemon).
```
Penggunaan: ./starterkit --decrypt
```

##### Kode
```
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
```

---

#### C. Memindahkan File: `--quarantine` dan `--return`
Karena Kanade adalah orang yang sangat pemalas (kecuali jika membuat musik), maka tambahkan juga fitur untuk memindahkan file yang ada pada directory starter kit ke directory karantina, dan begitu juga sebaliknya.
```
Penggunaan:
./starterkit --quarantine (pindahkan file dari directory starter kit ke karantina)
./starterkit --return (pindahkan file dari directory karantina ke starter kit)
```

##### Kode
```void move_files(const char *from, const char *to, const char *log_format) {
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
```

##### Penjelasan
- Nama file yang dienkripsi akan didekripsi dulu sebelum dipindahkan.
- Aktivitas dipantau dan dicatat di `activity.log`.

---

#### D. Menghapus File Karantina: `--eradicate`
Ena memberikan ide kepada mereka untuk menambahkan fitur untuk menghapus file - file yang ada pada directory karantina. 
Mendengar ide yang bagus tersebut, Kanade pun mencoba untuk menambahkan fitur untuk menghapus seluruh file yang ada di dalam directory karantina.
```
Penggunaan: ./starterkit --eradicate
```

##### Kode
```void delete_quarantine_files() {
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
```

---

#### E. Mematikan Daemon: `--shutdown`
Karena tagihan listrik Kanade sudah membengkak dan tidak ingin komputernya menyala secara terus - menerus, 
ia ingin program decrypt nama file miliknya dapat dimatikan secara aman berdasarkan PID dari proses program tersebut.
```	
Penggunaan: ./starterkit --shutdown
```

##### Kode
```
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
```

---

#### F. Error Handling
Mafuyu dan Kanade juga ingin program mereka dapat digunakan dengan aman dan nyaman tanpa membahayakan penggunanya sendiri, 
mengingat Mizuki yang masih linglung setelah keluar dari labirin Santerra De Laponte. 
Oleh karena itu, tambahkan error handling sederhana untuk mencegah penggunaan yang salah pada program tersebut.

##### Kode
```
if (argc != 2) {
    printf("Penggunaan:\n ./starterkit --decrypt\n ./starterkit --quarantine\n ./starterkit --return\n ./starterkit --eradicate\n ./starterkit --shutdown\n");
    return 1;
}
```

---

#### G. Logging Aktivitas
Terakhir, untuk mencatat setiap penggunaan program ini, Kanade beserta Mafuyu ingin menambahkan log dari setiap penggunaan program ini dan menyimpannya ke dalam file bernama activity.log.
Format:
```
    i] Decrypt: 
	[dd-mm-YYYY][HH:MM:SS] - Successfully started decryption process with PID <pid>.

	ii] Quarantine:
	[dd-mm-YYYY][HH:MM:SS] - <nama file> - Successfully moved to quarantine directory.

	iii] Return:
	[dd-mm-YYYY][HH:MM:SS] - <nama file> - Successfully returned to starter kit directory.

	iv] Eradicate:
	[dd-mm-YYYY][HH:MM:SS] - <nama file> - Successfully deleted.

	v] Shutdown:
	[dd-mm-YYYY][HH:MM:SS] - Successfully shut off decryption process with PID <pid>.
```

---

#### H. Kesimpulan
Program ini dirancang untuk membantu Kanade dalam menganalisis file berbahaya, memisahkan file mencurigakan, dan menjaga sistem tetap aman. Setiap fitur dilengkapi logging dan keamanan dasar sehingga praktis digunakan.

## Soal 4
### Oleh: Revalina Erica Permatasari

Suatu hari, Nobita menemukan sebuah alat aneh di laci mejanya. Alat ini berbentuk robot kecil dengan mata besar yang selalu berkedip-kedip. Doraemon berkata, "Ini adalah Debugmon! Robot super kepo yang bisa memantau semua aktivitas di komputer!" Namun, alat ini harus digunakan dengan hati-hati. Jika dipakai sembarangan, bisa-bisa komputer Nobita malah error total!

#### a.	Mengetahui semua aktivitas user
Doraemon ingin melihat apa saja yang sedang dijalankan user di komputernya. Maka, dia mengetik:
**./debugmon list <user>**
Debugmon langsung menampilkan daftar semua proses yang sedang berjalan pada user tersebut beserta PID, command, CPU usage, dan memory usage.

##### Code
```
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
```

#### b. Memasang mata-mata dalam mode daemon
Doraemon ingin agar Debugmon terus memantau user secara otomatis. Doraemon pun menjalankan program ini secara daemon dan melakukan pencatatan ke dalam file log dengan menjalankan:
**./debugmon daemon <user>**

##### Code
```
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
```

#### c.	Menghentikan pengawasan
User mulai panik karena setiap gerak-geriknya diawasi! Dia pun memohon pada Doraemon untuk menghentikannya dengan:
**./debugmon stop <user>**

##### Code 
```
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
```

#### d.	Menggagalkan semua proses user yang sedang berjalan
Doraemon yang iseng ingin mengerjai user dengan mengetik:
**./debugmon fail <user>**
Debugmon langsung menggagalkan semua proses yang sedang berjalan dan menulis status proses ke dalam file log dengan status FAILED. Selain menggagalkan, user juga tidak bisa menjalankan proses lain dalam mode ini.

##### Code

#### e.	Mengizinkan user untuk kembali menjalankan proses
Karena kasihan, Shizuka meminta Doraemon untuk memperbaiki semuanya. Doraemon pun menjalankan:
**./debugmon revert <user>**
Debugmon kembali ke mode normal dan bisa menjalankan proses lain seperti biasa.

##### Code

#### f.	Mencatat ke dalam file log
Sebagai dokumentasi untuk mengetahui apa saja yang debugmon lakukan di komputer user, debugmon melakukan pencatatan dan penyimpanan ke dalam file debugmon.log untuk semua proses yang dijalankan dengan format
**[dd:mm:yyyy]-[hh:mm:ss]_nama-process_STATUS(RUNNING/FAILED)**
Untuk poin b, c, dan e, status proses adalah RUNNING. Sedangkan untuk poin d, status proses adalah FAILED. 

##### Code
```
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
```
