#include "musteri.h"

Musteri::Musteri()
    : Kullanici(), tcKimlik(""), telefon(""), adres("") {
    rol = "musteri";
}

Musteri::Musteri(QString ad, QString soyad, QString email,
                 QString sifre, QString tcKimlik, QString telefon)
    : Kullanici(ad, soyad, email, sifre, "musteri"),
    tcKimlik(tcKimlik), telefon(telefon), adres("") {}

QString Musteri::bilgiGoster() const {
    return QString("MUSTERI: %1 %2 | TC: %3 | Tel: %4")
        .arg(ad).arg(soyad).arg(tcKimlik).arg(telefon);
}
