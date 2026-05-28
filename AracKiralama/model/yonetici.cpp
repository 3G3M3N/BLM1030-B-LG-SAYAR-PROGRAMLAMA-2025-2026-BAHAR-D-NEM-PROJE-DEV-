#include "yonetici.h"

Yonetici::Yonetici()
    : Kullanici(), yetkiSeviyesi(1), departman("") {
    rol = "yonetici";
}

Yonetici::Yonetici(QString ad, QString soyad, QString email,
                   QString sifre, int yetki)
    : Kullanici(ad, soyad, email, sifre, "yonetici"),
    yetkiSeviyesi(yetki), departman("Genel") {}

QString Yonetici::bilgiGoster() const {
    return QString("YONETICI: %1 %2 | Yetki: %3 | Dept: %4")
        .arg(ad).arg(soyad).arg(yetkiSeviyesi).arg(departman);
}
