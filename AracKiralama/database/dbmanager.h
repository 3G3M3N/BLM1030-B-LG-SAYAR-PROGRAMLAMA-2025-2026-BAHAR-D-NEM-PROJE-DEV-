#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QMap>

#include "../model/arac.h"
#include "../model/musteri.h"
#include "../model/yonetici.h"
#include "../model/kiralama.h"
#include "../model/odeme.h"

class DbManager {
private:
    QSqlDatabase db;
    static DbManager* instance;
    DbManager();

public:
    static DbManager* getInstance();

    bool baglan(const QString& dbYolu);
    void tablolarOlustur();

    bool        aracEkle(Arac& arac);
    bool        aracGuncelle(const Arac& arac);
    bool        aracSil(int id);
    QList<Arac> tumAraclariGetir();
    QList<Arac> musaitAraclariGetir();

    bool           kullaniciKaydet(Kullanici* k);
    Kullanici*     girisYap(const QString& email, const QString& sifre);
    QList<Musteri> tumMusterileriGetir();

    bool            kiralamaOlustur(Kiralama& k);
    bool            kiralamaKapat(int kiralamaId);
    QList<Kiralama> musteriKiralamalari(int musteriId);
    QList<Kiralama> tumKiralalar();

    bool bakimKaydet(int aracId, QDate tarih,
                     QString aciklama, double maliyet);
    bool bakimdenCikar(int aracId);
    QList<QMap<QString,QString>> tumBakimlariGetir();

    // Kart işlemleri
    bool    kartKaydet(int musteriId, QString kartNo,
                       QString kartAd, QString sonKullanma,
                       QString cvv);
    QMap<QString,QString> kartGetir(int musteriId);

    // Ödeme işlemleri
    bool            odemeKaydet(Odeme& odeme);
    QList<QMap<QString,QString>> tumOdemeleriGetir();
    QList<QMap<QString,QString>> musteriOdemeleri(int musteriId);
};

#endif // DBMANAGER_H
