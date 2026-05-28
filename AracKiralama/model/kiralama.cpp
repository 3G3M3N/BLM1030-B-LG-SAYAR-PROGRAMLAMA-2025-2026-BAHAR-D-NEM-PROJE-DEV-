#include "kiralama.h"

Kiralama::Kiralama()
    : id(0), musteriId(0), aracId(0),
    baslangicTarihi(QDate::currentDate()),
    bitisTarihi(QDate::currentDate().addDays(1)),
    toplamTutar(0), durum("Aktif"), notlar("") {}

Kiralama::Kiralama(int musteriId, int aracId, QDate baslangic,
                   QDate bitis, double gunlukFiyat)
    : id(0), musteriId(musteriId), aracId(aracId),
    baslangicTarihi(baslangic), bitisTarihi(bitis),
    durum("Aktif"), notlar("") {
    int gun = baslangic.daysTo(bitis);
    toplamTutar = gun * gunlukFiyat;
}
