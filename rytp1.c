#include <stdio.h>//standart kutuphane
#include <stdlib.h>//yardýmcý fonksiyonlar icin
#include <time.h>//zamanla ilgili fonksiyonlar
#include <string.h>//dizi islemleri
#include <dirent.h>//dosya dizin islemleri

typedef struct {
    int fiyat, hazirlamaSuresi;
    char yemekAdi[15];
    char kullanici[20];
    char siparisZamaniStr[25]; // Siparis zamanini tutacak dizi
    char hazirlanmaZamaniStr[25]; // Hazirlanma zamanini tutacak dizi
    char siparisID[25];//sipairs numarasini ilerde yapacagiz. burada dizisini olusturduk.
    char Mevcut[15];
    char Durum[10];
    int asciNo;
} siparis;
siparis s1;

int sipidno=1;//siparis numarasinin baslangici

 //burada butun fonksiyonlar tanimlandi cunku calisirken sirayla calisacak
int anaMenu();
void yeniSiparis(const char *dosyaYolu);
void olusturSiparisID(char *siparisID, int siparisNumarasi);
int kullaniciAdiKontrolu(const char *dosyaYolu, const char *kullanici);
void mevcutSiparis(const char *dosyaYolu);
void oncekiSiparis(const char *dosyaYolu);

int main(){

    int secim = anaMenu();//anamenuyu cagir ve kullanicidn secimi al
    while (secim != 0) {//kullanici cikis yapana kadar devam
        switch (secim) {
            case 1: yeniSiparis("C:\\Users\\Cansu KAYALAR\\Desktop\\yemeklistesi.txt"); break;
            case 2: mevcutSiparis("C:\\Users\\Cansu KAYALAR\\Desktop\\mutfak.txt"); break;
            case 3: oncekiSiparis("C:\\Users\\Cansu KAYALAR\\Desktop\\siparisler.txt"); break;
            case 0: break;
            default: printf("Hatali secim!\n"); break;
        }
        secim = anaMenu();//tekrar menuye don
    }
    return 0;
}

//anamenudeki secenekler yer aliyor. kullanicidan yapmasini istedigi islemi aliyoruz.
int anaMenu() {
    int secim;
    printf("\nRestoran Yemek Takip Projesi (ReYeTaP)\n");
    printf("1. Yeni Siparis\n");
    printf("2. Mevcut Siparis Durumu\n");
    printf("3. Daha Onceki Siparislerim\n");
    printf("0. Cikis\n");

    printf("Seciminizi yapiniz: ");//kullanicidan secim nosunu aldik
    scanf("%d", &secim);
//terminali sifirladik temiz durmasi acisindan.
    system("cls");

    return secim;
}

void yeniSiparis(const char *dosyaYolu) {
    int numara = 0;//siparis vermek istediginde yemek numaralarini alacagiz
    char kullanici[20];//kullanici adi almak icin dizi tanimladik
    char parola[20]; // Kullanýcý parolasý almak için dizi tanýmladýk
    char onay;//onay icin
    int yemekbulundu = 0;//mevcutluk durumuna gore siparis onaylanacak
    int sira = 1;
    int mevcutSiparis = 0;//mevcut siparis kontrolu
    int hazirlamaSuresi = 0;

    //yemek listesini dosyadan cektigimizde her bir secenek kendi basligi altinda olacak.
    printf("Yemek listesi:\n");
    printf("%2s %16s %15s %22s\n", "NO", "YEMEK ADI", "FIYAT", "HAZIRLANMA SURESI");

    //yemeklistesi dosyayini sadece okuyacagimiz icin fopen ile beraber read (r) kullandik.
    FILE *dosya = fopen(dosyaYolu, "r");
    if (dosya != NULL) {
        while (fscanf(dosya, "%s%d%d%s", s1.yemekAdi, &s1.fiyat, &s1.hazirlamaSuresi, s1.Mevcut) == 4) {//dosyadan 4 adet veriyi oku.okuyabilirse geri donus degeri 4 olur . olmazsa dongu sonlanýr
            if (strcmp(s1.Mevcut, "mevcut") == 0) {// s1.mevcut dizisi "mevcut" a esitse 0 dondurur ve if blogu calisir
                printf("%2d %16s %13dTL %18ddk\n", sira, s1.yemekAdi, s1.fiyat, s1.hazirlamaSuresi);
                yemekbulundu = 1;//mevcut kelimesi dizide okunursa yemek var demektir
                sira++;
            }
        }
        fclose(dosya);
    } else {
        perror("Dosya acilamadi!");
        return;
    }

    if (!yemekbulundu) {
        printf("Mevcut yemek bulunamadi!\n");
        return;
    }

    do {//kullanici adi siparisler dosyasinda yer aldigi surece kullanici adi alinmis dondur.
        printf("Kullanici adinizi giriniz: ");
        scanf("%s", kullanici);
        printf("Parolanizi giriniz: ");
        scanf("%s", parola);

        if (kullaniciAdiKontrolu("C:\\Users\\Cansu KAYALAR\\Desktop\\kullanicilar.txt", kullanici)==1) {
            printf("Bu kullanici adi zaten alinmis. Lutfen baska bir kullanici adi girin.\n");
        } else {
            // Kullanýcý adý ve parolayý kullanýcýlar dosyasýna yaz
            FILE *kullaniciDosyasi = fopen("C:\\Users\\Cansu KAYALAR\\Desktop\\kullanicilar.txt", "a");
            if (kullaniciDosyasi != NULL) {
                fprintf(kullaniciDosyasi, "%s %s\n", kullanici, parola);
                fclose(kullaniciDosyasi);
            } else {
                perror("Kullanici dosyasi acilamadi!");
            }
            break;
        }
    } while (1);

    int toplamSiparis = sira - 1;//toplam mevcut yemek sayisi
    while (numara != -1) {
        printf("Siparis numarasini giriniz, bitirmek icin -1 tuslayiniz : ");
        scanf("%d", &numara);

        if (numara == -1){
            system("cls");
            break;
        }
        else if (numara < 1 || numara > toplamSiparis) {//1 ile toplam siparis sayisi arasindaki sayilar girilmediginde
            printf("Gecersiz giris!\n");
            continue;
        }
        //sectigimiz numarali yemegi okuyoruz.
        dosya = fopen(dosyaYolu, "r");
        sira = 1;
        if (dosya != NULL) {
            while (fscanf(dosya, "%s%d%d%s", s1.yemekAdi, &s1.fiyat, &s1.hazirlamaSuresi, s1.Mevcut) == 4) {
                if (strcmp(s1.Mevcut, "mevcut") == 0) {
                        if (sira == numara) { // Dosyadaki yemekler tekrar okunur ve sira degiskeni numara ile karsilastirilir. eslesme bulunursa mevcut siparis 1 olarak ayaralnir
                        mevcutSiparis = 1;
                        hazirlamaSuresi = s1.hazirlamaSuresi;
                            break;
                        }
                    sira++;
                }
            }
            fclose(dosya);
        } else {
            perror("Yemek dosyasi acilamadi!");
            continue;
        }

        if (!mevcutSiparis) {
            printf("Gecersiz siparis!\n");
            continue;
        }

        do {
            printf("Siparisi onayliyor musunuz? (E/H): ");
            scanf(" %c", &onay);

            if (onay == 'E' || onay == 'e') {
                printf("Siparis onaylandi.\n");
                break;
            } else if (onay == 'H' || onay == 'h') {
                printf("Siparis iptal edildi.\n");
                break;
            } else {
                printf("Gecersiz giris! Lutfen 'E' veya 'H' girin.\n");
            }
        } while (1);

        time_t simdiki_zaman;//anlýk zamani tutan degisken
        struct tm *zaman_bilgisi;//zaman bilgilerini daha ayrýntýlý tutuyoruz.struct yapisi farklý turde yapilari bir arada tutar
        char zaman_dizisi[20];//zamani string olarak tutacak dizi.

        time(&simdiki_zaman);//su aki zamani simdiki_zaman degiskenine atadik
        zaman_bilgisi = localtime(&simdiki_zaman);//simdiki zaman degerini yerel zamana donusturur ve zaman_bilgisi ne atar
        strftime(zaman_dizisi, sizeof(zaman_dizisi), "%Y.%m.%d-%H.%M", zaman_bilgisi);//zamani istenilen formata donusturduk
        strcpy(s1.siparisZamaniStr, zaman_dizisi);//zaman dizisi karakter dizisini sipariszamani dizisine kopyaladik

        // Hazirlanma zamani olarak siparis zamani + yemek hazirlama suresi eklenerek ayarla
        zaman_bilgisi->tm_min += hazirlamaSuresi;//mevcut dakikaya sureyi ekle
        mktime(zaman_bilgisi);//zaman bilgisi guncellenir
        strftime(zaman_dizisi, sizeof(zaman_dizisi), "%Y.%m.%d-%H.%M", zaman_bilgisi);
        strcpy(s1.hazirlanmaZamaniStr, zaman_dizisi);
        strcpy(s1.Durum,"beklemede");
        strcpy(s1.kullanici,kullanici);

        olusturSiparisID(s1.siparisID, sipidno);
        // Dosyaya yaz
        FILE *siparisDosyasi = fopen("C:\\Users\\Cansu KAYALAR\\Desktop\\siparisler.txt", "a");//dosyanin mevcut icerigine ekleme yapar
        if (siparisDosyasi != NULL) {
            fprintf(siparisDosyasi, "%10s ", s1.siparisID);
            fprintf(siparisDosyasi, "%s ", s1.yemekAdi);
            fprintf(siparisDosyasi, "%5d ", s1.fiyat);
            fprintf(siparisDosyasi, "%s %s ", s1.siparisZamaniStr, s1.hazirlanmaZamaniStr);
            fprintf(siparisDosyasi, "%s ", s1.Durum);
            fprintf(siparisDosyasi, "%s\n", s1.kullanici);
            fclose(siparisDosyasi);
        } else {
            perror("Siparis dosyasi acilamadi!");
        }
    }
    sipidno++;
}


// Kullanici adi kontrol fonksiyonu
int kullaniciAdiKontrolu(const char *dosyaYolu, const char *kullanici) {
    FILE *dosya = fopen(dosyaYolu, "r");
    if (dosya != NULL) {
        char kayitliKullanici[20];
        while (fscanf(dosya, "%s %*s", kayitliKullanici) == 1) { // Parolayý okumadan sadece kullanýcý adýný oku
            if (strcmp(kayitliKullanici, kullanici) == 0) {
                fclose(dosya);
                return 1;
            }
        }
        fclose(dosya);
    }
    return 0;
}

// Kullanici adi ve parola kontrol fonksiyonu
int parolaKontrolu(const char *dosyaYolu, const char *kullanici, const char *parola) {
    FILE *dosya = fopen(dosyaYolu, "r");
    if (dosya != NULL) {
        char kayitliKullanici[20];
        char kayitliParola[20];
        while (fscanf(dosya, "%s %s", kayitliKullanici, kayitliParola) == 2) {
            if (strcmp(kayitliKullanici, kullanici) == 0 && strcmp(kayitliParola, parola) == 0) {
                fclose(dosya);
                return 1;
            }
        }
        fclose(dosya);
    }
    return 0;
}

void olusturSiparisID(char *siparisID, int siparisNumarasi) {
    time_t simdiki_zaman;//su anki zamani tut
    struct tm *zaman_bilgisi;//zaman bilgisini yapilandir
    char tarih_dizisi[10];//tarihi bicimlendir
    char numara_dizisi[4];//siparis numarasini bicimlendir

    // su anki zamani al
    time(&simdiki_zaman);
    //zaman bilgisini yerel zaman olarak al
    zaman_bilgisi = localtime(&simdiki_zaman);

    // Tarih bilgisini bicimlendir ve tarih_dizisi ne yerlestir
    strftime(tarih_dizisi, sizeof(tarih_dizisi), "%d%m%y", zaman_bilgisi);

    //siparis numarasini diziye donustur
    sprintf(numara_dizisi, "%03d", siparisNumarasi);

    // Siparis ID'sini olustur
    strcpy(siparisID, "SIP");//SIP ile basla
    strcat(siparisID, tarih_dizisi);//tarih bilgisini ekle
    strcat(siparisID, "_");//_ karakterini ekle
    strcat(siparisID, numara_dizisi);//siparis numarasini ekle
}


void mevcutSiparis(const char *dosyaYolu) {
    char kullanici[20];
    char parola[20];
    int toplam = 0;
    double maxKalanSure = -1;  // En fazla kalan süreyi takip etmek için
    char maxKalanSureStr[25];  // En geç teslim zamani stringini saklamak için

    printf("Kullanici adinizi giriniz: ");
    scanf("%19s", kullanici);
    printf("Parolanizi giriniz: ");
    scanf("%19s", parola);

    // Kullanýcý adý ve parola kontrolü
    if (parolaKontrolu("C:\\Users\\Cansu KAYALAR\\Desktop\\kullanicilar.txt", kullanici, parola) == 0) {
        printf("Hatali kullanici adi veya parola!\n");
        return;
    }

    FILE *siparisDosyasi = fopen(dosyaYolu, "r");
    if (siparisDosyasi != NULL) {
        time_t simdikiZaman;
        time(&simdikiZaman);
        int kullaniciBulundu = 0;
        struct tm *zaman_bilgisi = localtime(&simdikiZaman);

        printf(" yemek adi\tfiyat \t  tahmini teslim    durum\n");
        while (fscanf(siparisDosyasi, "%s %s %d %d %s %s %s %s A%d", s1.siparisID, s1.yemekAdi, &s1.fiyat, &s1.hazirlamaSuresi, s1.siparisZamaniStr, s1.hazirlanmaZamaniStr, s1.Durum, s1.kullanici, &s1.asciNo) == 9) {
            if (strcmp(kullanici, s1.kullanici) == 0) {
                int yil, ay, gun, saat, dakika;
                if (sscanf(s1.hazirlanmaZamaniStr, "%d.%d.%d-%d.%d", &yil, &ay, &gun, &saat, &dakika) == 5) {
                    struct tm hazirlanmaZamani = {0};
                    hazirlanmaZamani.tm_year = yil - 1900;
                    hazirlanmaZamani.tm_mon = ay - 1;
                    hazirlanmaZamani.tm_mday = gun;
                    hazirlanmaZamani.tm_hour = saat;
                    hazirlanmaZamani.tm_min = dakika;
                    hazirlanmaZamani.tm_sec = 0;

                    time_t hazirlanmaZamaniEpoch = mktime(&hazirlanmaZamani);

                    if (hazirlanmaZamaniEpoch != -1) {
                        double zamanFarki = difftime(hazirlanmaZamaniEpoch, simdikiZaman);

                        if (zamanFarki > 0) {
                            printf("%13s %4dTL %13s %13s\n", s1.yemekAdi, s1.fiyat, s1.hazirlanmaZamaniStr, s1.Durum);
                            kullaniciBulundu = 1;
                            toplam += s1.fiyat;

                            if (zamanFarki > maxKalanSure) {
                                maxKalanSure = zamanFarki;
                                strcpy(maxKalanSureStr, s1.hazirlanmaZamaniStr);
                            }
                        }
                    }
                }
            }
        }

        printf("Sepet tutari: %d\n\n\n", toplam);
        if (maxKalanSure > 0) {
            int saat = (int)(maxKalanSure / 3600);
            int dakika = (int)((maxKalanSure - saat * 3600) / 60);
            printf("siparisiniz yaklasik %d saat %d dakika icinde hazir", saat, dakika);
        }
        if (!kullaniciBulundu) {
            printf("Mevcut siparisiniz bulunmamaktadir!\n");
        }
        fclose(siparisDosyasi);
    } else {
        perror("Siparis dosyasi acilamadi!");
    }
}

void oncekiSiparis(const char *dosyaYolu) {
    char kullanici[20]; // Kullanýcý adýný tutan dizi
    char parola[20]; // Parolayý tutan dizi
    int kullaniciBulundu = 0; // Kullanýcý bulunduðunda flag

    printf("Kullanici adinizi giriniz: ");
    scanf("%s", kullanici);
    printf("Parolanizi giriniz: ");
    scanf("%s", parola);

    // Kullanýcý adý ve parola kontrolü
    if (parolaKontrolu("C:\\Users\\Cansu KAYALAR\\Desktop\\kullanicilar.txt", kullanici, parola) == 0) {
        printf("Hatali kullanici adi veya parola!\n");
        return;
    }

    FILE *siparisDosyasi = fopen(dosyaYolu, "r");
    if (siparisDosyasi != NULL) {
        time_t simdikiZaman; // Þu anki zamaný tut
        time(&simdikiZaman); // Þu anki zamaný al
        // Bu iþlemi daha önce de yaptýk. Yerel zamaný güncelliyoruz.
        while (fscanf(siparisDosyasi, "%s %s %d %s %s %s %s", s1.siparisID, s1.yemekAdi, &s1.fiyat, s1.siparisZamaniStr, s1.hazirlanmaZamaniStr, s1.Durum, s1.kullanici) == 7) {
            if (strcmp(kullanici, s1.kullanici) == 0) {
                int yil, ay, gun, saat, dakika, saniye;
                if (sscanf(s1.hazirlanmaZamaniStr, "%d.%d.%d-%d.%d.%d", &yil, &ay, &gun, &saat, &dakika) == 5) {
                    struct tm hazirlanmaZamani = {0};
                    hazirlanmaZamani.tm_year = yil - 1900;
                    hazirlanmaZamani.tm_mon = ay - 1;
                    hazirlanmaZamani.tm_mday = gun;
                    hazirlanmaZamani.tm_hour = saat;
                    hazirlanmaZamani.tm_min = dakika;
                    hazirlanmaZamani.tm_sec = 0;

                    time_t hazirlanmaZamaniEpoch = mktime(&hazirlanmaZamani);

                    if (hazirlanmaZamaniEpoch != -1) {
                        double zamanFarki = difftime(simdikiZaman, hazirlanmaZamaniEpoch);
                        if (zamanFarki > 0) {
                            printf("%15s %5dTL %20s\n", s1.yemekAdi, s1.fiyat, s1.siparisZamaniStr);
                            kullaniciBulundu = 1;
                        }
                    }
                }
            }
        }
        if (!kullaniciBulundu) {
            printf("Gecmis siparisiniz bulunmamaktadir!\n");
        }
        fclose(siparisDosyasi);
    } else {
        perror("Siparis dosyasi acilamadi!");
    }
}
