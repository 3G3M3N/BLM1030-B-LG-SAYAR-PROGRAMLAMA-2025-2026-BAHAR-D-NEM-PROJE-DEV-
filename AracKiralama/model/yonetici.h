#ifndef YONETICI_H
#define YONETICI_H

#include "kullanici.h"

// Kullanici'dan turetilen yonetici sinifi
class Yonetici : public Kullanici {
private:
    int     yetkiSeviyesi;
    QString departman;

public:
    Yonetici();
    Yonetici(QString ad, QString soyad, QString email,
             QString sifre, int yetki = 1);

    int     getYetkiSeviyesi() const { return yetkiSeviyesi; }
    QString getDepartman()     const { return departman; }
    void    setDepartman(QString d)  { departman = d; }
    bool    superAdminMi()     const { return yetkiSeviyesi >= 2; }

    QString bilgiGoster() const override;
};

#endif // YONETICI_H
