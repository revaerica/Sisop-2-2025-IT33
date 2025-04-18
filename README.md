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

### Dokumentasi
Dokumentasi pengerjaan nomor 1 <br> <br>

#### [1] Jika dijalankan ./action
Perintah ``./action`` akan mendownload dan unzip dari link google drive yang isi filenya ada folder CLues beserta isinya folder
![nomor1](assets/101%20action.png) <br>

### [2] Filter Folder
Perintah ``./action -m Filter`` akan memfilter file dengan 5 huruf / (1.txt) dan (a.tx) , 1 sampai 6 dan a sampai f yang ada di semua folder Clues, lalu menghapus semua file yang tidak memenuhi ketentuan tersebut. <br>
![desc](assets/102%20Filter.png) <br>

### [3] Menyatukan isi file dari txt
Perintah ``./action -m Combine`` akan menjadikan satu seluruh isi file yang ada di folder Filtered / isi dalam file txt ke dalam file Combined.txt <br>
![desc](assets/103.1%20Combine.png) <br>
Dijadikan satu semua isinya, tetapi masih dalam keadaan huruf yang asli / belum di rot13
![desc](assets/103.2%20Isi%20Combine.png) <br>

### [4] Decode 
Perintah ``./action -m Decode`` akan menjalankan rot13 dari file Combine dan mencari 13 huruf setelahnya dalam file Combine dan hasilnya ada di file Decoded.txt <br>
![desc](assets/104.1%20Decode.png) <br>
Isi dari file Decode.txt <br>
![desc](assets/104.2%20Isi%20Decode.png) <br>

### [5] Input password
Jika password sudah terlihat, saya input ke link lokasi yang telah disediakan <br>
Berikut tampilan password yang saya input dan ternyata benar <br>
![desc](assets/105%20Input%20Password.png) <br>

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

### Dokumentasi
Dokumentasi pengerjaan nomor 2 <br> <br>

#### [1] Jika dijalankan ./starterkit
Perintah ``./starterkit`` akan mendownload dan unzip dari link google drive dan akan membuat folder starter_kit untuk menyimpan seluruh file dari link tersebut, tetapi di dalam folder starter_kit nama file masih terenkripsi, belum nama file yang jelas <br>
![desc](assets/201%20starterkit.png) <br>

### [2] Decrypt
Perintah ``./starterkit --decrypt `` akan menampilkan PID baru yang sedang dijalankan saat itu dan menampilkan di dalam file log <br>
![desc](assets/202%20decrypt.png) <br>

### [3] Quarantine
Perintah ``./starterkit --quarantine`` akan memindahkan file dari folder starter_kit ke dalam folder quarantine dan sudah tidak terenkripsi atau nama file sudah jelas <br>
![desc](assets/203.1%20quarantine.png) <br>
Menampilkan aktivitas perpindahan file ke dalam activity.log
![desc](assets/203.2%20log%20quarantine.png) <br>

### [4] Return
Perintah ``./starterkit --return`` akan memindahkan file dari folder quarantine ke dalam folder starter_kit lagi dan sudah tidak terenkripsi atau nama file sudah jelas <br>
![desc](assets/204.1%20return.png) <br>
Menampilkan aktivitas perpindahan file ke dalam activity.log
![desc](assets/204.2%20log%20return.png) <br>

### [5] Eradicate
Menghapus file yang ada pada folder quarantine, disini saya mencoba membuat file baru didalam quarantine "halooo.txt" <br>
![desc](assets/205%20eradicate.png) <br>

### [6] Shutdown
Mematikan PID yang sedang berjalan dan mencatat ke dalam activity.log <br>
![desc](assets/206%20shutdown.png) <br>

### [7] Error Handling
Output / arahan jika user salah memasukkan perintah <br>
![desc](assets/207%20eror%20handling.png) <br>

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
 
     printf("Process list for user %s:\n", user);
     printf("PID\tCOMMAND\t\tCPU(s)\tMEM(KB)\n");
 
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
             printf("%s\t%s\t%.1f\t%lu\n", ent->d_name, name, cpu_time, mem);
             tulisLog(name, "RUNNING");
         }
     }
     closedir(proc);
 }
```
Fungsi listProcess berfungsi untuk menampilkan daftar proses milik user tertentu yang sedang berjalan di sistem Linux, beserta informasi PID, COMMAND, CPU usage, dan Memory usage
1. Ambil UID user target menggunakan fungsi ``get_uid()``. Jika gagal, fungsi langsung keluar.
2. Buka direktori ``/proc``, yaitu direktori virtual di Linux yang berisi informasi proses. Jika gagal dibuka, tampilkan pesan error.
3. Tampilkan header tabel untuk daftar proses.
4. Loop ke semua entitas dalam ``/proc``, dan hanya proses yang nama direktori-nya berupa angka (yaitu PID) yang diproses.
5. Buka file ``/proc/<pid>/status`` untuk membaca informasi proses tersebut.
   Dari file status, baca:
   - Uid: untuk mengetahui pemilik proses.
   - Name: untuk nama proses.
   - VmRSS: untuk memori yang dipakai proses (dalam KB).
6. Jika UID proses sesuai dengan target, lanjut proses Buka file ``/proc/<pid>/stat`` untuk membaca waktu CPU proses.
7. Ambil nilai utime (waktu CPU di user mode) dan stime (di kernel mode).
8. Hitung total waktu CPU dalam detik menggunakan rumus **(utime + stime) / sysconf(_SC_CLK_TCK)**
9. Tampilkan informasi proses ke terminal: PID, COMMAND, waktu CPU, dan memori.
10. Tulis log status proses ke log file menggunakan fungsi ``tulisLog()`` dengan status RUNNING.
11. Setelah semua proses dibaca, tutup direktori ``/proc``.

##### Dokumentasi
<img width="878" alt="image" src="https://github.com/user-attachments/assets/bc3dc091-acbd-4abd-a0b1-232f233899c3" />

##### Revisi Code
Menambahkan presentase CPU dan MEM
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
1. Mengambil UID dari nama user pakai ``get_uid()``. Kalau UID tidak ketemu, fungsi langsung keluar.
2. Membuka direktori ``/proc`` untuk baca semua proses.
3. Baca total memori sistem dari ``/proc/meminfo`` (ambil MemTotal).
4. Baca total waktu CPU sistem dari ``/proc/stat`` (ambil baris cpu).
5. Iterasi semua entry di /proc:
   a. Cek kalau nama folder-nya angka (berarti PID proses)
   b. Baca ``/proc/[pid]/status``
     - Ambil UID proses
     - Ambil COMMAND
     - Ambil penggunaan memori RSS (VmRSS)
7. Kalau UID-nya sama dengan UID user target:
   a. Baca ``/proc/[pid]/stat``
   b. Ambil waktu CPU user (utime) dan kernel (stime) dan hitung:
      - **Waktu CPU (detik) = (utime + stime) / sysconf(_SC_CLK_TCK)**
      - **Persentase CPU = (utime + stime) / totalCpuTime × 100**
      - **Persentase Memori = VmRSS / MemTotal × 100**
8. Menampilkan PID, COMMAND, Waktu CPU, Memori, Persentase CPU, dan Persentase Memori ke terminal
9. Tulis log status proses (panggil tulisLog() dengan status RUNNING)
10. Tutup direktori /proc

##### Dokumentasi
<img width="818" alt="image" src="https://github.com/user-attachments/assets/3d7b725b-0e8b-494f-8462-83be36a1543a" />

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
daemonUser() membuat background service yang
- Jalan terus di background
- Tiap 5 detik cek semua proses di ``/proc``
- Kalau proses milik user target → tulis log “RUNNING”
- Gak ngapa-ngapain kalau user-nya sedang diblokir (cek pakai ``isUserBlocked()``)
  1. Fork proses
     a. Kalau fork gagal → exit.
     b. Kalau di parent >> menyimpan PID child ke file ``PIDFILE_FMT``. lalu Parent keluar.
  2. Setup daemon di child
     a. Set ``umask(0)``.
     b. Buat session baru manggunakan ``setsid()``.
     c. Pindah ke root directory /.
     d. Tutup stdin, stdout, stderr biar lepas dari terminal.
  3. Mengambil UID target menggunakan ``get_uid(user)``.
  4. Loop terus menerus menggunakan ``while(1)``.
  5. Di tiap iterasi
     a. Cek apakah user sedang diblokir pakai ``isUserBlocked(user)``.
     b. Kalau tidak diblokir maka
        - Membuka direktori ``/proc``.
        - Lalu iterasi semua folder di ``/proc``. Skip yang bukan angka (folder proses PID harus angka).
        - Buka file ``/proc/[pid]/status``.
        - Baca>> **Uid: → UID proses** dan **Name: → Nama proses**.
        - Kalau UID proses = UID target, maka panggil ``tulisLog(name, "RUNNING")``.
        - Tutup ``/proc``.
        - Tunggu 5 detik sebelum ulang lagi agar daemon tidak boros CPU

##### Dokumentasi
<img width="559" alt="image" src="https://github.com/user-attachments/assets/4e2f1c5e-a8b2-496e-979c-63bd93017537" />

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
Fungsi dari ``stopProses()`` adalah untuk menghentikan daemon debugmon yang lagi monitor user tertentu. Caranya dengan baca PID daemon dari file, lalu kill proses-nya.
1. Bikin path file PID user.
   ``PIDFILE_FMT`` itu format string buat simpen path file PID, misal: ``/var/run/debugmon_%s.pid``
2. Cek apakah file PID ada.
3. Kalau nggak ada → tampil “No active daemon found.”
4. Baca isi file (PID daemon).
5. Kirim sinyal ``SIGTERM`` ke PID daemon.
   a. Kalau berhasil
      - Hapus file PID.
      - Tulis log ``debugmon_stop``.
      - Tampil “Monitoring stopped.”
   b. Kalau gagal
      - Tampil pesan error pakai ``perror()``.


##### Dokumentasi
<img width="524" alt="image" src="https://github.com/user-attachments/assets/63ab52cd-cb10-4289-8d5e-b3d211904140" />

#### d.	Menggagalkan semua proses user yang sedang berjalan
Doraemon yang iseng ingin mengerjai user dengan mengetik:
**./debugmon fail <user>**
Debugmon langsung menggagalkan semua proses yang sedang berjalan dan menulis status proses ke dalam file log dengan status FAILED. Selain menggagalkan, user juga tidak bisa menjalankan proses lain dalam mode ini.

##### Code
```
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
```
Pada failProcess ini yaitu Menggagalkan Semua Proses User (./debugmon fail <user>)
1. Saat command ./debugmon fail <user> dijalankan
2. Debugmon mencari semua proses milik user tersebut.
3. Semua proses yang sedang berjalan langsung di-KILL (kecuali proses penting kayak debugmon, bash, zsh, systemd, dll).
4. Setiap proses yang dibunuh dicatat ke file log dengan status FAILED.
5. Dibuatkan file lock (lockfile) untuk user itu → artinya user tidak bisa lagi menjalankan proses lain selama lockfile ada.
6. Proses daemon-nya akan ngecek lockfile ini → kalau ketemu, daemon-nya berhenti ngelog karena user udah diblokir.

##### Dokumentasi
<img width="959" alt="image" src="https://github.com/user-attachments/assets/7d5dd0fb-167f-4af3-9d6b-a2b742af5230" />


#### e.	Mengizinkan user untuk kembali menjalankan proses
Karena kasihan, Shizuka meminta Doraemon untuk memperbaiki semuanya. Doraemon pun menjalankan:
**./debugmon revert <user>**
Debugmon kembali ke mode normal dan bisa menjalankan proses lain seperti biasa.

##### Code
```
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
```
Fungsi revertProcess() adalah Membuka blokir user yang sebelumnya dibuat oleh failProcess() dengan cara menghapus lockfile milik user tersebut.
1. Susun path lockfile /var/lock/debugmon_<user>.lock.
2. Hapus file tersebut.
3. Kalau berhasil
   a. Catat log debugmon_revert RUNNING.
   b. Tampilkan pesan user bisa akses normal.
4. Kalau gagal (file gak ada) akan menampilkan pesan gak ada blokir.

##### Dokumentasi
<img width="408" alt="image" src="https://github.com/user-attachments/assets/b4b18b4e-dd24-44bc-970c-40f208c0ccc9" />

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
Fungsi tulisLog adalah untuk mencatat aktivitas atau status proses ke dalam file log.
1. Buka file log LOGFILE dalam mode append ("a").
2. Jika gagal, tampilkan pesan error dan keluar.
3. Ambil waktu lokal saat ini (time() dan localtime()).
4. Tulis log ke file dengan format:
   **[dd:mm:yyyy]-[HH:MM:SS]_procName_status**
5. Tutup file log.

##### Dokumentasi
<img width="773" alt="image" src="https://github.com/user-attachments/assets/8f6bbc4b-95bb-403b-984a-306d40a86d6a" />
<img width="733" alt="image" src="https://github.com/user-attachments/assets/1786b42c-e168-4bee-b124-b67b3447d96b" />
<img width="623" alt="image" src="https://github.com/user-attachments/assets/696c7335-485a-4cbd-acf3-1cbec78bb709" />

##### Kendala
1. Di nomor 4 ini fungsi failProcess dan revertProcess sebenarnya dilakukan revisi. Saya sudah sempat merevisi, tetapi saat difail dia tidak keluar dari User dan tetap berada di terminal. Lalu saya tetap menggunakan code awal untuk failProcess dan revertProcess yang berhasil keluar dari user serta me-revert dari user serta root.
2. Untuk nomor 3, saya sempat mencoba mengerjakan dengan menginstall virtualbox dan Ubuntu terlebih dahulu. Namun, saya terus terkena malware dan ada keterbatasan waktu juga.
   ![Screenshot 2025-04-11 072932](https://github.com/user-attachments/assets/1bcecaee-4aa8-4427-9ddb-f43d160c799f)

   ![Screenshot 2025-04-12 155537](https://github.com/user-attachments/assets/9e6118ea-087a-43f7-9899-5685cba57472)
