#ifndef KULLANICI_H
#define KULLANICI_H

#include <QString>

// temel kullanici sinifi, musteri ve yonetici burdan turetilecek
class Kullanici {
protected:
    int     id;
    QString ad;
    QString soyad;
    QString email;
    QString sifre;
    QString rol;

public:
    Kullanici();
    Kullanici(QString ad, QString soyad, QString email,
              QString sifre, QString rol);
    virtual ~Kullanici();

    int     getId()    const { return id; }
    QString getAd()    const { return ad; }
    QString getSoyad() const { return soyad; }
    QString getEmail() const { return email; }
    QString getSifre() const { return sifre; }
    QString getRol()   const { return rol; }

    void setId(int i)        { id = i; }
    void setAd(QString a)    { ad = a; }
    void setSoyad(QString s) { soyad = s; }
    void setEmail(QString e) { email = e; }
    void setSifre(QString s) { sifre = s; }

    // saf sanal - alt siniflar override etmek zorunda
    virtual QString bilgiGoster() const = 0;
};

#endif // KULLANICI_H
