#include "kullanici.h"

Kullanici::Kullanici()
    : id(0), ad(""), soyad(""), email(""), sifre(""), rol("") {}

Kullanici::Kullanici(QString ad, QString soyad, QString email,
                     QString sifre, QString rol)
    : id(0), ad(ad), soyad(soyad), email(email),
    sifre(sifre), rol(rol) {}

Kullanici::~Kullanici() {}
