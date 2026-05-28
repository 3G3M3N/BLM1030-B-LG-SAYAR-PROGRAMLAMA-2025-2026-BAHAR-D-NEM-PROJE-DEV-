#include "odeme.h"

Odeme::Odeme()
    : id(0), kiralamaId(0), musteriId(0),
    tutar(0), tarih(QDate::currentDate()),
    durum("Tamamlandi"), kartSonDort("") {}

Odeme::Odeme(int kiralamaId, int musteriId,
             double tutar, QString kartSonDort)
    : id(0), kiralamaId(kiralamaId), musteriId(musteriId),
    tutar(tutar), tarih(QDate::currentDate()),
    durum("Tamamlandi"), kartSonDort(kartSonDort) {}
