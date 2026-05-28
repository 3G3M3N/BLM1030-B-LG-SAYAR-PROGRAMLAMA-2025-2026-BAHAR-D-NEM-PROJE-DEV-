#ifndef ARAC_H
#define ARAC_H

#include <QString>
#include <QDate>

class Arac {
private:
    int     id;
    QString marka;
    QString model;
    int     yil;
    QString plaka;
    QString renk;
    QString kategori;
    double  gunlukFiyat;
    bool    musait;
    int     km;
    QString durum;

public:
    Arac();
    Arac(QString marka, QString model, int yil, QString plaka,
         QString renk, QString kategori, double fiyat);

    int     getId()          const { return id; }
    QString getMarka()       const { return marka; }
    QString getModel()       const { return model; }
    int     getYil()         const { return yil; }
    QString getPlaka()       const { return plaka; }
    QString getRenk()        const { return renk; }
    QString getKategori()    const { return kategori; }
    double  getGunlukFiyat() const { return gunlukFiyat; }
    bool    isMusait()       const { return musait; }
    int     getKm()          const { return km; }
    QString getDurum()       const { return durum; }

    void setId(int i)             { id = i; }
    void setMusait(bool m)        { musait = m; }
    void setKm(int k)             { if(k >= 0) km = k; }
    void setDurum(QString d)      { durum = d; }
    void setGunlukFiyat(double f) { if(f > 0) gunlukFiyat = f; }

    QString tamAd() const {
        return marka + " " + model + " (" + QString::number(yil) + ")";
    }
};

#endif // ARAC_H
