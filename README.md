# Sisop-2-2025-IT33

#### Nama Anggota
1. Ni'mah Fauziyyah Atok (5027241103)
2. Revalina Erica Permatasari (5027241007)

## Daftar Isi
1. [Soal 1](#soal-1)
2. [Soal 2](#soal-2)
4. [Soal 4](#soal-4)

## Soal 1 
## Soal 2
### Oleh: Ni'mah Fauziyyah A

### Soal 
``
Kamu terbangun secara tiba-tiba di suatu lokasi (https://dragon-pw-checker.vercel.app/) yang tidak diketahui, saat kamu melihat sekitar yang terlihat hanyalah kegelapan dan sebuah pintu dengan dua mata berwarna yang melihatmu dari lubang pintu tersebut. <br>

Ia merupakan naga bernama Cyrus yang menjaga pintu tersebut dan kamu harus meng-input password yang benar untuk masuk. 
Karena tidak mungkin untuk menebak password, Ia memberikanmu sebuah clue Clues.zip 
(https://drive.google.com/file/d/1xFn1OBJUuSdnApDseEczKhtNzyGekauK/view). 
Untungnya, kamu merupakan Directory Lister yang jago sehingga clue ini dapat berubah menjadi sebuah password.
``

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
#### F. Kesimpulan  
Program ini membantu "Observer" dalam menganalisis clues yang tersebar, memastikan hanya fragment yang valid diproses, dan menerjemahkan pesan tersembunyi dari Seeker menggunakan ROT13. Dengan modularitas fungsi, program lebih terstruktur dan mudah dikembangkan untuk kebutuhan misi selanjutnya.
## Soal 4
