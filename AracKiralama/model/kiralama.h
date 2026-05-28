#ifndef KIRALAMA_H
#define KIRALAMA_H

#include <QString>
#include <QDate>

class Kiralama {
private:
    int     id;
    int     musteriId;
    int     aracId;
    QDate   baslangicTarihi;
    QDate   bitisTarihi;
    double  toplamTutar;
    QString durum;
    QString notlar;

public:
    Kiralama();
    Kiralama(int musteriId, int aracId, QDate baslangic,
             QDate bitis, double gunlukFiyat);

    int     getId()          const { return id; }
    int     getMusteriId()   const { return musteriId; }
    int     getAracId()      const { return aracId; }
    QDate   getBaslangic()   const { return baslangicTarihi; }
    QDate   getBitis()       const { return bitisTarihi; }
    double  getToplamTutar() const { return toplamTutar; }
    QString getDurum()       const { return durum; }
    QString getNotlar()      const { return notlar; }

    void setId(int i)        { id = i; }
    void setDurum(QString d) { durum = d; }
    void setNotlar(QString n){ notlar = n; }

    int gunSayisi() const {
        return baslangicTarihi.daysTo(bitisTarihi);
    }
};

#endif // KIRALAMA_H
