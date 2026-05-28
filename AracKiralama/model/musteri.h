#ifndef MUSTERI_H
#define MUSTERI_H

#include "kullanici.h"

// Kullanici'dan turetilen musteri sinifi
class Musteri : public Kullanici {
private:
    QString tcKimlik;
    QString telefon;
    QString adres;

public:
    Musteri();
    Musteri(QString ad, QString soyad, QString email,
            QString sifre, QString tcKimlik, QString telefon);

    QString getTcKimlik() const { return tcKimlik; }
    QString getTelefon()  const { return telefon; }
    QString getAdres()    const { return adres; }
    void setAdres(QString a)    { adres = a; }
    void setTelefon(QString t)  { telefon = t; }

    QString bilgiGoster() const override;
};

#endif // MUSTERI_H
