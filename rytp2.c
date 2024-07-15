#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#define MAX_YEMEK_SAYISI 100//max yemek sayisini belirledik ki degistirmek istersek kolay olsun

typedef struct siparis {
    int fiyat, hazirlamaSuresi;
    char yemekAdi[15];
    char kullanici[20];
    char siparisZamaniStr[25];
    char hazirlanmaZamaniStr[25];
    char siparisID[15];
    char Mevcut[15];
    char Durum[10];
    int asciNo;
} siparis;
siparis s1;//s1 : siparis turunde olan ve icinde siparise ait bilgiler tutan degisken

//siparis kuyruk yapisi
typedef struct {
    siparis siparisler[MAX_YEMEK_SAYISI];
    int on;
    int arka;
    int sayac;
} siparisKuyrugu;

//yemek tuketim miktari
typedef struct {
    char yemekAdi[15];
    int tuketimMiktari;
} YemekTuketim;


typedef struct {
    char tarih[11];
    int kazanc;
} GunlukKazanc;

typedef struct {
    char kullanici[20];
    int siparisMiktari;
} KullaniciSiparis;

int anaMenu();
int ekleguncellesil();
int analiz();//gunluk haftalik kazanclar

//yemek ekleme fonksiyonu
void yemekEkle(const char *dosyaYolu);

//yemek guncelleme fonksiyonu
void yemekGuncelle(const char *dosyaYolu);

//yemek silme fonksiyonu
void yemekSil(const char *dosyaYolu);

//siparis onay/red fonksiyonu
void siparisOnayRed(const char *dosyaYolu);

//gun sonu rapor fonksiyonu
void gunsonu(const char *kaynakDosyaYolu);

void raporGoruntule();

// donemlik kazanclar
void gunlukKazanc(const char *klasorYolu);

void aylikKazanc(const char *dosyaYolu);

void donemKazanci(const char *kaynakKlasor);

void enCokTuketim(const char *kaynakKlasor);

void mutfak(const char *siparisDosyaAdi, const char *yemekListesiDosyaAdi);

//yemekleri dosyadan getirir
void yemekleriOku(const char *dosyaAdi, siparis yemekler[], int *yemekSayisi) {
    FILE *dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        perror("Dosya acilamadi");
        return;
    }
    //dosya sonuna kadar okur
    while (fscanf(dosya, "%s %d %d %s", yemekler[*yemekSayisi].yemekAdi, &yemekler[*yemekSayisi].fiyat, &yemekler[*yemekSayisi].hazirlamaSuresi, yemekler[*yemekSayisi].Mevcut) == 4) {
        //okunan satirdaki durum "mevcut" ise yemek sayisini artir
        if (strcmp(yemekler[*yemekSayisi].Mevcut, "mevcut") == 0) {//isaret edilen satirdaki yemek durumunun mevcutluk durumu
            (*yemekSayisi)++;
        }
    }

    fclose(dosya);
}

//hazirlanma suresini yemek listesinden okuduk
int hazirlanmaSuresiBul(siparis yemekler[], int yemekSayisi, const char *yemekAdi) {
    for (int i = 0; i < yemekSayisi; i++) {
        //yemekler dizisinin i. elemaninin yemekadi alaninin verilen yemekadi ile uyumluluguna bakar
        if (strcmp(yemekler[i].yemekAdi, yemekAdi) == 0 && strcmp(yemekler[i].Mevcut, "mevcut") == 0) {
            return yemekler[i].hazirlamaSuresi;
        }
    }
    return -1;
}
//gelen siparisi duruma gore kuyruga ekliyoruz
void kuyrugaEkle(siparisKuyrugu *k, siparis s) {
    if (k->sayac < MAX_YEMEK_SAYISI) {//eger sayac kuyruk ile max sipaaris sayisi arasındaysa kyruga ekle
        // Siparisi kuyruga ekle
        k->siparisler[k->arka] = s;
        // Kuyruk indeksini guncelle
        k->arka = (k->arka + 1) % MAX_YEMEK_SAYISI;
        k->sayac++;
    } else {
        printf("Kuyruk dolu, siparis eklenemiyor!\n");
    }
}
siparis kuyruktanCikar(siparisKuyrugu *k) {
    siparis s = {0, "", -1, 0}; // Bos bir siparis olusturulur.

    if (k->sayac > 0) { // Kuyrukta siparis var mi?
        s = k->siparisler[k->on]; // Kuyrugun onundeki siparisi al.
        k->on = (k->on + 1) % MAX_YEMEK_SAYISI; // Kuyrugun ön isaretcisini bir sonraki siparise kaydir.
        k->sayac--; // Kuyruktaki siparis sayisini bir azalt.
    } else {
        printf("Kuyruk bos, cikarilacak siparis yok!\n"); // Kuyruk boşsa, mesaj yazdır.
    }

    return s; // Siparisi geri döndür.
}


int main() {
    int secim;
    //restorandan secim aldiktan sonra hangi dosya acilacak
    while((secim = anaMenu()) != 0) {
        switch(secim) {
            case 1: {
                int secim_ = ekleguncellesil();
                switch(secim_) {
                    case 1: yemekEkle("C:\\Users\\Cansu KAYALAR\\Desktop\\yemeklistesi.txt"); break;
                    case 2: yemekGuncelle("C:\\Users\\Cansu KAYALAR\\Desktop\\yemeklistesi.txt"); break;
                    case 3: yemekSil("C:\\Users\\Cansu KAYALAR\\Desktop\\yemeklistesi.txt"); break;
                    case 0: break;
                    default: printf("Hatali secim!\n"); break;
                }
            } break;
            case 2: siparisOnayRed("C:\\Users\\Cansu KAYALAR\\Desktop\\siparisler.txt"); break;
            case 3: gunsonu("C:\\Users\\Cansu KAYALAR\\Desktop\\siparisler.txt"); break;
            case 4: raporGoruntule(); break;
            case 5: {
                int _secim=analiz();
                switch(_secim) {
                    case 1: gunlukKazanc("C:\\Users\\Cansu KAYALAR\\Desktop\\gunsonurapor"); break;
                    case 2: aylikKazanc("C:\\Users\\Cansu KAYALAR\\Desktop\\gunsonurapor");  break;
                    case 3: donemKazanci("C:\\Users\\Cansu KAYALAR\\Desktop\\gunsonurapor"); break;
                    case 4: enCokTuketim("C:\\Users\\Cansu KAYALAR\\Desktop\\gunsonurapor"); break;
                    case 0: break;
                    default: printf("Hatali secim!\n"); break;
                }
            } break;
            case 6: mutfak("C:\\Users\\Cansu KAYALAR\\Desktop\\siparisler.txt","C:\\Users\\Cansu KAYALAR\\Desktop\\yemeklistesi.txt"); break;
            default: printf("Hatali secim!\n"); break;
        }
    }
    return 0;
}

int anaMenu(){
    int secim;
    printf("Restoran Yonetim Sistemi\n");
    printf("1. Yemek Ekle-Guncelle-Sil\n");
    printf("2. Yeni Siparis ONAY/RED\n");
    printf("3. Rapor Al\n");
    printf("4. Rapor Goruntule\n");
    printf("5. Analizler\n");
    printf("6. Mutfak\n");
    printf("0. Cikis\n");
    printf("Seciminizi giriniz: "); scanf("%d", &secim);
    system("cls");
         return secim;
}
int ekleguncellesil(){
    int secim_;
    printf("1. Yemek Ekle\n");
    printf("2. Yemek Guncelle\n");
    printf("3. Yemek Sil\n");
    printf("0. Anamenu\n");
    printf("Seciminizi giriniz: "); scanf("%d", &secim_);
    system("cls");
        return secim_;

}
int analiz(){
    int _secim;
    printf("1. Gunluk Kazanc\n");
    printf("2. Aylik Kazanc\n");
    printf("3. Donem kazanci\n");
    printf("4. En Cok Tuketim\n");
    printf("0. Cikis\n");
    printf("Seciminizi giriniz: "); scanf("%d", &_secim);
    system("cls");
        return _secim;
}
void yemekEkle(const char *dosyaYolu) {

    printf("Yemek adini giriniz: ");
    scanf("%49s", s1.yemekAdi);
    printf("Yemek fiyatini giriniz: ");
    scanf("%d", &s1.fiyat);
    printf("Yemek hazirlama suresini giriniz (dakika): ");
    scanf("%d", &s1.hazirlamaSuresi);
    printf("Yemek durumunu giriniz (mevcut/mevcut degil): ");
    scanf("%15s", s1.Mevcut);

    // Dosyaya yazma iþlemi
    FILE *dosya = fopen(dosyaYolu, "a+");
    if (dosya == NULL) {
        perror("Dosya acilamadi!");
        exit(EXIT_FAILURE);
    }
    fprintf(dosya, "%s %d %d %s\n", s1.yemekAdi, s1.fiyat, s1.hazirlamaSuresi, s1.Mevcut);
    fclose(dosya);
    printf("Yemek basariyla eklendi.\n");

}

void yemekGuncelle(const char *dosyaYolu) {
    char arananYemek[50];
    int bulundu = 0;
    int sira = 1;

    printf("Yemek listesi:\n");
    printf("%2s %16s %15s %22s %10s\n", "NO", "YEMEK ADI", "FIYAT", "HAZIRLANMA SURESI","DURUM");

    FILE *dosya = fopen(dosyaYolu, "r");
    if (dosya != NULL) {
        while (fscanf(dosya, "%49s %d %d %15s", s1.yemekAdi, &s1.fiyat, &s1.hazirlamaSuresi, s1.Mevcut) == 4) {

                printf("%2d %16s %13dTL %18ddk %13s\n", sira, s1.yemekAdi, s1.fiyat, s1.hazirlamaSuresi, s1.Mevcut);
                sira++;

        }
        fclose(dosya);
    } else {
        perror("Dosya acilamadi!");
        return;
    }

    printf("Guncellenecek yemek adini giriniz: ");
    scanf("%49s", arananYemek);

    dosya = fopen(dosyaYolu, "r");
    if (dosya == NULL) {
        perror("Dosya acilamadi!");
        exit(EXIT_FAILURE);
    }
    //eski dosyadaki verileri temp dosyasina aktarip, temp dosyasini yeni dosya yapiyoruz
    FILE *tempDosya = fopen("temp.txt", "w");
    if (tempDosya == NULL) {
        perror("Gecici dosya acilamadi!");
        fclose(dosya);
        exit(EXIT_FAILURE);
    }

    while (fscanf(dosya, "%49s %d %d %15s", s1.yemekAdi, &s1.fiyat, &s1.hazirlamaSuresi, s1.Mevcut) == 4) {
        if (strcmp(s1.yemekAdi, arananYemek) == 0) {
            printf("Yeni fiyatini giriniz: ");
            scanf("%d", &s1.fiyat);
            printf("Yeni hazirlama suresini giriniz (dakika): ");
            scanf("%d", &s1.hazirlamaSuresi);
            printf("Yeni durumunu giriniz (mevcut/mevcutdegil): ");
            scanf("%15s", s1.Mevcut);
            bulundu = 1;
        }
        fprintf(tempDosya, "%s %d %d %s\n", s1.yemekAdi, s1.fiyat, s1.hazirlamaSuresi, s1.Mevcut);
    }

    fclose(dosya);
    fclose(tempDosya);
        system("cls");

        //guncelleyecgimiz yemek bulunduysa
    if (bulundu) {
        remove(dosyaYolu);
        rename("temp.txt", dosyaYolu);
        printf("Yemek basariyla guncellendi.\n");
    } else {
        remove("temp.txt");
        printf("Yemek bulunamadi!\n");
    }
}

void yemekSil(const char *dosyaYolu) {
    char silinecekYemek[50];
    int sira = 1;

    printf("Yemek listesi:\n");
    printf("%2s %16s %15s %22s %10s\n", "NO", "YEMEK ADI", "FIYAT", "HAZIRLANMA SURESI", "DURUM");

    FILE *dosya = fopen(dosyaYolu, "r");
    if (dosya != NULL) {
        while (fscanf(dosya, "%49s %d %d %15s", s1.yemekAdi, &s1.fiyat, &s1.hazirlamaSuresi, s1.Mevcut) == 4) {
            printf("%2d %16s %13dTL %18ddk %13s\n", sira, s1.yemekAdi, s1.fiyat, s1.hazirlamaSuresi, s1.Mevcut);
            sira++;
        }
        fclose(dosya);
    } else {
        perror("Dosya acilamadi!");
        return;
    }

    printf("Silinecek yemek adini giriniz: ");
    scanf("%49s", silinecekYemek);

    dosya = fopen(dosyaYolu, "r");
    if (dosya == NULL) {
        perror("Dosya acilamadi!");
        exit(EXIT_FAILURE);
    }

        //yemegi sildiktten sonra guncel listeyi bu dosyaya yaziyoruz
    FILE *tempDosya = fopen("temp.txt", "w");
    if (tempDosya == NULL) {
        perror("Gecici dosya acilamadi!");
        fclose(dosya);
        exit(EXIT_FAILURE);
    }

    int bulundu = 0;
    while (fscanf(dosya, "%49s %d %d %15s", s1.yemekAdi, &s1.fiyat, &s1.hazirlamaSuresi, s1.Mevcut) == 4) {
        if (strcmp(s1.yemekAdi, silinecekYemek) != 0) {
            fprintf(tempDosya, "%s %d %d %s\n", s1.yemekAdi, s1.fiyat, s1.hazirlamaSuresi, s1.Mevcut);
        } else {
            bulundu = 1;
        }
    }

    fclose(dosya);
    fclose(tempDosya);

    if (bulundu) {
        remove(dosyaYolu);
        rename("temp.txt", dosyaYolu);//gecici dosyayi has dosyanin ismiyle degistir
        printf("Yemek basariyla silindi.\n");
    } else {
        remove("temp.txt");
        printf("Yemek bulunamadi!\n");
    }
}

void siparisOnayRed(const char *dosyaYolu) {
    FILE *dosya = fopen(dosyaYolu, "r");
    if (dosya == NULL) {
        perror("Dosya acilamadi!");
        exit(EXIT_FAILURE);
    }

    FILE *tempDosya = fopen("temp.txt", "w");
    if (tempDosya == NULL) {
        perror("Gecici dosya acilamadi!");
        fclose(dosya);
        exit(EXIT_FAILURE);
    }
    int onay = 0;

    // Siparisleri oku, listele oanyla reddet
    while (fscanf(dosya, "%s %s %d %s %s %s %s", s1.siparisID, s1.yemekAdi, &s1.fiyat, s1.siparisZamaniStr, s1.hazirlanmaZamaniStr, s1.Durum, s1.kullanici) == 7) {
        if (strcmp(s1.Durum, "beklemede") == 0) {
            printf("Siparis ID: %s, Yemek Adi: %s, Fiyat: %d, Durum: %s, Kullanici: %s\n", s1.siparisID, s1.yemekAdi, s1.fiyat, s1.Durum, s1.kullanici);
            printf("Onaylamak icin 1, reddetmek icin 0, islemi bitirmek icin -1 giriniz: ");
            scanf("%d", &onay);

            if (onay == 1) {
                strcpy(s1.Durum, "onaylandi");
            } else if (onay == 0) {
                strcpy(s1.Durum, "reddedildi");
            } else if (onay == -1) {
                break;
            }

            //gecici dosyadan verileri yazdirdi
            fprintf(tempDosya, "%s %s %d %s %s %s %s\n", s1.siparisID, s1.yemekAdi, s1.fiyat, s1.siparisZamaniStr, s1.hazirlanmaZamaniStr, s1.Durum, s1.kullanici);
        } else {
            // Durumu beklemede olmayan siparisleri oldugu gibi kopyala
            fprintf(tempDosya, "%s %s %d %s %s %s %s\n", s1.siparisID, s1.yemekAdi, s1.fiyat, s1.siparisZamaniStr, s1.hazirlanmaZamaniStr, s1.Durum, s1.kullanici);
        }
    }

    fclose(dosya);
    fclose(tempDosya);

    // Orijinal dosyayi gecici dosya ile degistir
    remove(dosyaYolu);
    rename("temp.txt", dosyaYolu);

    if (onay != -1) {
        printf("Siparislerin durumu guncellendi.\n");
    } else {
        printf("Siparis onay/red islemi sonlandirildi.\n");
    }
}

void gunsonu(const char *kaynakDosyaYolu) {
    // Sistem tarihini al
    time_t simdikiZaman = time(NULL);
    struct tm *zamanBilgisi = localtime(&simdikiZaman);
    char tarih[11];
    strftime(tarih, sizeof(tarih), "%Y.%m.%d", zamanBilgisi); // Saat bilgisi olmadan tarih formatı

    // Kaynak dosyayi ac (siparisler dosyasindan oku)
    FILE *kaynakDosya = fopen(kaynakDosyaYolu, "r");
    if (kaynakDosya == NULL) {
        perror("Kaynak dosya acilamadi!");
        exit(EXIT_FAILURE);
    }

    // Hedef dosya adini olustur ve ac (gunsonurapor klasoru )
    char hedefDosyaAdi[50];

    sprintf(hedefDosyaAdi, "C:\\Users\\Cansu KAYALAR\\Desktop\\gunsonurapor\\Rapor_%s.txt", tarih);
    FILE *hedefDosya = fopen(hedefDosyaAdi, "w");

    if (hedefDosya == NULL) {
        perror("Hedef dosya acilamadi!");

        exit(EXIT_FAILURE);
    }

    // Kayıtları filtrele ve yeni dosyaya yaz (gunsonuraporunun icine ne yazacagiz)
    char satir[100];
    while (fgets(satir, sizeof(satir), kaynakDosya) != NULL) {
        sscanf(satir, "%s %s %d %s %s %s %s",
               s1.siparisID, s1.yemekAdi, &s1.fiyat,
               s1.siparisZamaniStr, s1.hazirlanmaZamaniStr, s1.Durum, s1.kullanici);
               char kayitTarihi[11];
               sscanf(s1.siparisZamaniStr, "%10s", kayitTarihi);

            // Eslesen siparisi hedef dosyaya yaz
            fprintf(hedefDosya, "%10s %13s %5d %13s %13s %13s %13s\n",
                    s1.siparisID, s1.yemekAdi, s1.fiyat,
                    s1.siparisZamaniStr, s1.hazirlanmaZamaniStr, s1.Durum, s1.kullanici);
    }

    fflush(hedefDosya);//dosya kapatilmadan once tum verilerin gectiginden emin olmak icin kontrol

    fclose(kaynakDosya);
    freopen(kaynakDosyaYolu, "w", kaynakDosya);// siparisler dosyasinin icini bosaltilir
    fclose(hedefDosya);

    printf("Gunluk rapor '%s' dosyasina basariyla kaydedildi.\n", hedefDosyaAdi);
}

void raporGoruntule() {
    char tarih[11];
    printf("Goruntulemek istediginiz raporun tarihini YYYY.MM.DD formatinda giriniz: ");
    scanf("%10s", tarih);

    char raporDosyaAdi[50];
    sprintf(raporDosyaAdi, "C:\\Users\\Cansu KAYALAR\\Desktop\\gunsonurapor\\Rapor_%s.txt", tarih);//rapordosyadi'na gunsonuraporuna tarihi yerlestirip gonderdik

    FILE *raporDosyasi = fopen(raporDosyaAdi, "r");
    if (raporDosyasi == NULL) {
        printf("Belirtilen tarihe ait rapor bulunamadi veya dosya acilamadi.\n");
        return;
    }

    // Rapor dosyasinin icerigini oku ve ekrana yaz
    char satir[256];
    while (fgets(satir, sizeof(satir), raporDosyasi) != NULL) {
        printf("%s", satir);
    }

    // Dosyayi kapat
    fclose(raporDosyasi);
}

void gunlukKazanc(const char *klasorYolu) {
    char tarih[11];
    printf("Gunluk kazanci gormek istediginiz tarihi YYYY.MM.DD formatinda giriniz: ");
    scanf("%10s", tarih);

    char dosyaAdi[256];
    sprintf(dosyaAdi, "%s\\Rapor_%s.txt", klasorYolu, tarih);

    FILE *dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        perror("Dosya acilamadi!");
        return;
    }

    int toplamKazanc = 0;
    while (fscanf(dosya, "%s %s %d %s %s %s %s",
                  s1.siparisID, s1.yemekAdi, &s1.fiyat,
                  s1.siparisZamaniStr,
                  s1.hazirlanmaZamaniStr, s1.Durum, s1.kullanici) == 7) {
        if (strcmp(s1.Durum,"onaylandi")==0) {
            toplamKazanc += s1.fiyat;
        }
    }

    fclose(dosya);
    printf("%10s tarihine ait Toplam kazanc: %d TL\n",tarih ,toplamKazanc);
}

void aylikKazanc(const char *kaynakKlasor) {
    // Ayın gun sayısını hesapla
    int gunSayisi;
    int yil,ay;

    printf("Lutfen hesaplamak istediginiz yili ve ayi giriniz (YYYY.MM): "); scanf("%d.%d", &yil, &ay);
    if (ay == 2) {
        if ((yil % 4 == 0 && yil % 100 != 0) || yil % 400 == 0) {
            gunSayisi = 29; // Artık yıl kontrolu
        } else {
            gunSayisi = 28;
        }
    } else if (ay == 4 || ay == 6 || ay == 9 || ay == 11) {
        gunSayisi = 30;
    } else {
        gunSayisi = 31;
    }

    // Aylık kazanç
    int aylikkazanc = 0;

    // Dosya işlemleri için gerekli degişkenler
    char dosyaAdi[100];
    char raporYolu[100];

    // Dosyaları tarayarak aylık kazancı hesapla
    for (int gun = 1; gun <= gunSayisi; gun++) {
        sprintf(dosyaAdi, "Rapor_%04d.%02d.%02d.txt", yil, ay, gun);
        sprintf(raporYolu, "%s/%s", kaynakKlasor, dosyaAdi);

        // Dosyayı ac
        FILE *dosya = fopen(raporYolu, "r");
        if (dosya == NULL) {
            continue; // Dosya yoksa bir sonraki gune gec
        }

        // Dosyadan her bir satırı oku ve fiyatları topla
        while (fscanf(dosya, "%s %s %d %s %s %s %s",
                      s1.siparisID, s1.yemekAdi, &s1.fiyat,
                      s1.siparisZamaniStr, s1.hazirlanmaZamaniStr,
                      s1.Durum, s1.kullanici) == 7) {
            if (strcmp(s1.Durum, "onaylandi") == 0) {
                aylikkazanc += s1.fiyat;
            }
        }

        // Dosyayı kapat
        fclose(dosya);
    }

    // Aylık kazancı ekrana yazdır
    printf("Aylik Kazanc (%04d.%02d): %d TL\n", yil, ay, aylikkazanc);
}

void donemKazanci(const char *kaynakKlasor) {
    char baslangicTarihi[11], bitisTarihi[11];

    // Kullanıcıdan başlangıç ve bitiş tarihlerini al
    printf("Lutfen donemin baslangic tarihini YYYY.MM.DD formatinda giriniz: ");
    scanf("%10s", baslangicTarihi);
    printf("Lutfen donemin bitis tarihini YYYY.MM.DD formatinda giriniz: ");
    scanf("%10s", bitisTarihi);

     //baslangic ve bitis zamanlarinin turlerini time_tm turune donusturmek ıcın parcaladik
    struct tm baslangic = {0}, bitis = {0};
    sscanf(baslangicTarihi, "%d.%d.%d", &baslangic.tm_year, &baslangic.tm_mon, &baslangic.tm_mday);
    sscanf(bitisTarihi, "%d.%d.%d", &bitis.tm_year, &bitis.tm_mon, &bitis.tm_mday);
    baslangic.tm_year -= 1900; // tm_year, 1900'den itibaren yılları sayar
    baslangic.tm_mon -= 1; // tm_mon, 0'dan itibaren ayları sayar
    bitis.tm_year -= 1900;
    bitis.tm_mon -= 1;

    int toplamKazanc = 0;
    char dosyaAdi[100];
    char raporYolu[100];
    time_t baslangicZaman = mktime(&baslangic);
    time_t bitisZaman = mktime(&bitis);

    // Başlangıc ve bitis zamanları arasındaki tum gunleri dolas
    for (time_t zaman = baslangicZaman; zaman <= bitisZaman; zaman += 86400) { // Her gun icin 86400 saniye eklenir
        struct tm *tm = localtime(&zaman);
        strftime(dosyaAdi, sizeof(dosyaAdi), "Rapor_%Y.%m.%d.txt", tm);//tarih bilgisini diziye yaz
        sprintf(raporYolu, "%s/%s", kaynakKlasor, dosyaAdi);

        FILE *dosya = fopen(raporYolu, "r");
        if (dosya == NULL) {
            continue; // Dosya yoksa bir sonraki güne geç
        }

        // Dosyadan her bir satırı oku ve fiyatları topla
        while (fscanf(dosya, "%s %s %d %s %s %s %s",
                      s1.siparisID, s1.yemekAdi, &s1.fiyat,
                      s1.siparisZamaniStr, s1.hazirlanmaZamaniStr,
                      s1.Durum, s1.kullanici) == 7) {
            if (strcmp(s1.Durum,"onaylandi")==0){
                    toplamKazanc += s1.fiyat;
            }
        }

        fclose(dosya);
    }

    printf("%10s - %10s tarihleri arasi toplam kazanc: %d TL\n",baslangicTarihi,bitisTarihi ,toplamKazanc);
}

void enCokTuketim(const char *kaynakKlasor) {
    DIR *klasor = opendir(kaynakKlasor);
    struct dirent *dosya;
    YemekTuketim yemekler[MAX_YEMEK_SAYISI] = {0};
    int yemekSayisi = 0;
    GunlukKazanc gunlukKazanclar[365] = {0}; // Yılın her günü için bir kayıt varsayımıyla
    int gunSayisi = 0;
    KullaniciSiparis kullaniciSiparisler[MAX_YEMEK_SAYISI] = {0};
    int kullaniciSayisi = 0;
    double toplamHazirlamaSuresi = 0.0;

    if (klasor == NULL) {
        perror("Klasor acilamadi!");
        return;
    }
//***********************************************************************************************************************************************

   //klasorun icini oku
    while ((dosya = readdir(klasor)) != NULL) {

        if (strstr(dosya->d_name, "Rapor_") == dosya->d_name) {
            char dosyaYolu[256];
            sprintf(dosyaYolu, "%s/%s", kaynakKlasor, dosya->d_name);
            FILE *raporDosyasi = fopen(dosyaYolu, "r");

            if (raporDosyasi == NULL) {
                continue;
            }
            while (fscanf(raporDosyasi, "%s %s %d %s %s %s %s",
                          s1.siparisID, s1.yemekAdi, &s1.fiyat,
                          s1.siparisZamaniStr, s1.hazirlanmaZamaniStr,
                          s1.Durum, s1.kullanici) == 7) {
                if (strcmp(s1.Durum, "onaylandi") == 0) {
                    // Yemek tuketim miktarını guncelle
                    int bulundu = 0;
                    for (int i = 0; i < yemekSayisi; i++) {
                        if (strcmp(yemekler[i].yemekAdi, s1.yemekAdi) == 0) {
                            yemekler[i].tuketimMiktari++;
                            bulundu = 1;
                            break;
                        }
                    }
                    if (!bulundu) {
                        strcpy(yemekler[yemekSayisi].yemekAdi, s1.yemekAdi);
                        yemekler[yemekSayisi].tuketimMiktari = 1;
                        yemekSayisi++;
                    }

                    // Günlük kazancı güncelle
                    char tarih[11]; // Saat bilgisi olmadan YYYY-MM-DD formatında
                    sscanf(s1.siparisZamaniStr, "%10s", &tarih[0], &tarih[1], &tarih[2]);
                    bulundu = 0;
                    for (int i = 0; i < gunSayisi; i++) {
                        if (strcmp(gunlukKazanclar[i].tarih, tarih) == 0) {
                            gunlukKazanclar[i].kazanc += s1.fiyat;
                            bulundu = 1;
                            break;
                        }
                    }
                    if (!bulundu) {
                        strcpy(gunlukKazanclar[gunSayisi].tarih, tarih);
                        gunlukKazanclar[gunSayisi].kazanc = s1.fiyat;
                        gunSayisi++;
                    }

                    // Kullanıcı siparis miktarını guncelle
                    bulundu = 0;
                    for (int i = 0; i < kullaniciSayisi; i++) {
                        if (strcmp(kullaniciSiparisler[i].kullanici, s1.kullanici) == 0) {
                            kullaniciSiparisler[i].siparisMiktari++;
                            bulundu = 1;
                            break;
                        }
                    }
                    if (!bulundu) {
                        strcpy(kullaniciSiparisler[kullaniciSayisi].kullanici, s1.kullanici);
                        kullaniciSiparisler[kullaniciSayisi].siparisMiktari = 1;
                        kullaniciSayisi++;
                    }

                    // Hazırlama suresini zaman farki alarak hesapla
                    struct tm siparisZamani = {0}, hazirlanmaZamani = {0};
                    sscanf(s1.siparisZamaniStr, "%d.%d.%d-%d.%d.%d",
                           &siparisZamani.tm_year, &siparisZamani.tm_mon, &siparisZamani.tm_mday,
                           &siparisZamani.tm_hour, &siparisZamani.tm_min, &siparisZamani.tm_sec);
                    siparisZamani.tm_year -= 1900;
                    siparisZamani.tm_mon -= 1;

                    sscanf(s1.hazirlanmaZamaniStr, "%d.%d.%d-%d.%d.%d",
                           &hazirlanmaZamani.tm_year, &hazirlanmaZamani.tm_mon, &hazirlanmaZamani.tm_mday,
                           &hazirlanmaZamani.tm_hour, &hazirlanmaZamani.tm_min, &hazirlanmaZamani.tm_sec);
                    hazirlanmaZamani.tm_year -= 1900;
                    hazirlanmaZamani.tm_mon -= 1;

                    time_t siparisEpoch = mktime(&siparisZamani);
                    time_t hazirlanmaEpoch = mktime(&hazirlanmaZamani);

                    double farkSaniye = difftime(hazirlanmaEpoch, siparisEpoch);//iki zamani saniye farkindan hesaplar
                    toplamHazirlamaSuresi += farkSaniye / 3600.0;
                }
            }

            fclose(raporDosyasi);
        }
    }
    closedir(klasor);

    // En çok tüketilen yemeği bul
    YemekTuketim enCokTuketilen = {"", 0};
    for (int i = 0; i < yemekSayisi; i++) {
        if (yemekler[i].tuketimMiktari > enCokTuketilen.tuketimMiktari) {
            enCokTuketilen = yemekler[i];
        }
    }

    // En kazançlı günü bul
    GunlukKazanc enKazancli = {"", 0};
    for (int i = 0; i < gunSayisi; i++) {
        if (gunlukKazanclar[i].kazanc > enKazancli.kazanc) {
            enKazancli = gunlukKazanclar[i];
        }
    }

    // En çok sipariş veren kullanıcıyı bul
    KullaniciSiparis enCokSiparis = {"", 0};
    for (int i = 0; i < kullaniciSayisi; i++) {
        if (kullaniciSiparisler[i].siparisMiktari > enCokSiparis.siparisMiktari) {
            enCokSiparis = kullaniciSiparisler[i];
        }
    }

    printf("En Cok Tuketilen Yemek: %s (%d adet)\n", enCokTuketilen.yemekAdi, enCokTuketilen.tuketimMiktari);
    printf("En Kazancli Gun: %s (%d TL)\n", enKazancli.tarih, enKazancli.kazanc);
    printf("En Cok Siparis Veren Kullanici: %s (%d siparis)\n", enCokSiparis.kullanici, enCokSiparis.siparisMiktari);
    printf("Toplam Yemek Hazirlama Suresi: %.1f saat\n", toplamHazirlamaSuresi);
}

void mutfak(const char *siparisDosyaAdi, const char *yemekListesiDosyaAdi) {
    siparis yemekler[MAX_YEMEK_SAYISI] = {0};
    siparis s;
    int yemekSayisi = 0;

    yemekleriOku(yemekListesiDosyaAdi, yemekler, &yemekSayisi);

    FILE *siparisDosyasi = fopen(siparisDosyaAdi, "r");
    if (siparisDosyasi == NULL) {
        perror("Siparis dosyasi acilamadi");
        return;
    }

    FILE *mutfakDosyasi = fopen("C:\\Users\\Cansu KAYALAR\\Desktop\\mutfak.txt", "w");
    if (mutfakDosyasi == NULL) {
        perror("Mutfak dosyasi acilamadi");
        fclose(siparisDosyasi);
        return;
    }

    siparisKuyrugu kuyruk = {{0}, 0, 0, 0};
    time_t asciBitisZamanlari[3] = {0};

    while (fscanf(siparisDosyasi, "%s %s %d %s %s %s %s", s.siparisID, s.yemekAdi, &s.fiyat,
                  s.siparisZamaniStr, s.hazirlanmaZamaniStr, s.Durum, s.kullanici) == 7) {
        if (strcmp(s.Durum, "onaylandi") == 0) {
            s.hazirlamaSuresi = hazirlanmaSuresiBul(yemekler, yemekSayisi, s.yemekAdi);
            if (s.hazirlamaSuresi != -1) {
                struct tm tmSiparis = {0}, tmHazirlanma = {0};
                int gunSiparis, aySiparis, yilSiparis, saatSiparis, dakikaSiparis;
                int gunHazirlanma, ayHazirlanma, yilHazirlanma, saatHazirlanma, dakikaHazirlanma;

                if (sscanf(s.siparisZamaniStr, "%d.%d.%d-%d.%d", &yilSiparis, &aySiparis, &gunSiparis, &saatSiparis, &dakikaSiparis) == 5 &&
                    sscanf(s.hazirlanmaZamaniStr, "%d.%d.%d-%d.%d", &yilHazirlanma, &ayHazirlanma, &gunHazirlanma, &saatHazirlanma, &dakikaHazirlanma) == 5) {

                    tmSiparis.tm_year = yilSiparis - 1900;
                    tmSiparis.tm_mon = aySiparis - 1;
                    tmSiparis.tm_mday = gunSiparis;
                    tmSiparis.tm_hour = saatSiparis;
                    tmSiparis.tm_min = dakikaSiparis;
                    tmSiparis.tm_sec = 0;
                    tmSiparis.tm_isdst = -1;

                    tmHazirlanma.tm_year = yilHazirlanma - 1900;
                    tmHazirlanma.tm_mon = ayHazirlanma - 1;
                    tmHazirlanma.tm_mday = gunHazirlanma;
                    tmHazirlanma.tm_hour = saatHazirlanma;
                    tmHazirlanma.tm_min = dakikaHazirlanma;
                    tmHazirlanma.tm_sec = 0;
                    tmHazirlanma.tm_isdst = -1;

                    time_t siparisZamaniEpoch = mktime(&tmSiparis);
                    time_t hazirlanmaZamaniEpoch = mktime(&tmHazirlanma);

                    if (difftime(hazirlanmaZamaniEpoch, siparisZamaniEpoch) >= 0) {
                        if (asciBitisZamanlari[0] == 0) {
                            for (int i = 0; i < 3; i++) {
                                asciBitisZamanlari[i] = siparisZamaniEpoch;
                            }
                        }

                        int enErkenBitenAsciIndex = 0;
                        time_t enErkenBitisZamani = asciBitisZamanlari[0];
                        for (int i = 1; i < 3; i++) {
                            if (asciBitisZamanlari[i] < enErkenBitisZamani) {
                                enErkenBitenAsciIndex = i;
                                enErkenBitisZamani = asciBitisZamanlari[i];
                            }
                        }

                        time_t yeniHazirlanmaZamani;
                        if (siparisZamaniEpoch > enErkenBitisZamani) {
                            yeniHazirlanmaZamani = siparisZamaniEpoch + (s.hazirlamaSuresi * 60);
                        } else {
                            yeniHazirlanmaZamani = enErkenBitisZamani + (s.hazirlamaSuresi * 60);
                        }

                        asciBitisZamanlari[enErkenBitenAsciIndex] = yeniHazirlanmaZamani;
                        s.asciNo = enErkenBitenAsciIndex + 1;

                        struct tm *tmYeniHazirlanma = localtime(&yeniHazirlanmaZamani);
                        if (tmYeniHazirlanma != NULL) {
                            sprintf(s.hazirlanmaZamaniStr, "%02d.%02d.%d-%02d.%02d",
                                    tmYeniHazirlanma->tm_year + 1900, tmYeniHazirlanma->tm_mon + 1, tmYeniHazirlanma->tm_mday,
                                    tmYeniHazirlanma->tm_hour, tmYeniHazirlanma->tm_min);
                        } else {
                            perror("localtime error");
                            continue;
                        }

                        kuyrugaEkle(&kuyruk, s);
                    } else {
                        printf("Siparis zamani ve hazirlanma zamani uygun degil: Siparis ID: %s\n", s.siparisID);
                    }
                } else {
                    printf("Hatali siparis zamani formati: %s\n", s.siparisZamaniStr);
                }

            } else {
                printf("Yemek listesinde '%s' bulunamadi.\n", s.yemekAdi);
            }

        }
    }

    fclose(siparisDosyasi);

    while (kuyruk.sayac > 0) {
        siparis cikarilanSiparis = kuyruktanCikar(&kuyruk);
        fprintf(mutfakDosyasi, "%10s %12s %4d %4d %13s %13s %10s %10s A%d\n", cikarilanSiparis.siparisID, cikarilanSiparis.yemekAdi, cikarilanSiparis.fiyat, cikarilanSiparis.hazirlamaSuresi, cikarilanSiparis.siparisZamaniStr, cikarilanSiparis.hazirlanmaZamaniStr, cikarilanSiparis.Durum, cikarilanSiparis.kullanici, cikarilanSiparis.asciNo);
    }

    fclose(mutfakDosyasi);
}
