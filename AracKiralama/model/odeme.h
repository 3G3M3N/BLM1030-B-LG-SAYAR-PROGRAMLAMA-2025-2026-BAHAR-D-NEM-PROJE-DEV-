#ifndef ODEME_H
#define ODEME_H

#include <QString>
#include <QDate>

class Odeme {
private:
    int     id;
    int     kiralamaId;
    int     musteriId;
    double  tutar;
    QDate   tarih;
    QString durum;
    QString kartSonDort;

public:
    Odeme();
    Odeme(int kiralamaId, int musteriId,
          double tutar, QString kartSonDort);

    int     getId()          const { return id; }
    int     getKiralamaId()  const { return kiralamaId; }
    int     getMusteriId()   const { return musteriId; }
    double  getTutar()       const { return tutar; }
    QDate   getTarih()       const { return tarih; }
    QString getDurum()       const { return durum; }
    QString getKartSonDort() const { return kartSonDort; }

    void setId(int i)        { id = i; }
    void setDurum(QString d) { durum = d; }
};

#endif // ODEME_H
