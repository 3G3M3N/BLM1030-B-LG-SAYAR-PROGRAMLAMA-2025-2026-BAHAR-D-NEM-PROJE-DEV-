#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include "database/dbmanager.h"
#include "model/yonetici.h"
#include "model/arac.h"

namespace Ui { class AdminPanel; }

class AdminPanel : public QMainWindow {
    Q_OBJECT

public:
    explicit AdminPanel(Yonetici* yonetici, QWidget *parent = nullptr);
    ~AdminPanel();

private slots:
    void on_btnAracEkle_clicked();
    void on_btnAracSil_clicked();
    void on_btnBakimEkle_clicked();
    void on_btnBakimdenCikar_clicked();
    void on_btnKiralamaKapat_clicked();
    void on_btnCikis_clicked();           // Çıkış butonu
    void on_txtAracArama_textChanged(const QString& arananKelime);
    void araclariYukle();
    void kiralamalariYukle();
    void musterileriYukle();
    void bakimlariYukle();
    void odemeleriYukle();

private:
    Ui::AdminPanel *ui;
    Yonetici* aktifYonetici;
    QList<Arac> tumAraclar;
};

#endif // ADMINPANEL_H
