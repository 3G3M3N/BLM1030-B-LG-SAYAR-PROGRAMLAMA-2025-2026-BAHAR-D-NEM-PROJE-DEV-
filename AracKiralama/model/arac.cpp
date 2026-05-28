#include "arac.h"

Arac::Arac()
    : id(0), marka(""), model(""), yil(2020), plaka(""),
    renk(""), kategori(""), gunlukFiyat(0),
    musait(true), km(0), durum("Aktif") {}

Arac::Arac(QString marka, QString model, int yil, QString plaka,
           QString renk, QString kategori, double fiyat)
    : id(0), marka(marka), model(model), yil(yil), plaka(plaka),
    renk(renk), kategori(kategori), gunlukFiyat(fiyat),
    musait(true), km(0), durum("Aktif") {}
