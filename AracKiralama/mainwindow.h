#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include "database/dbmanager.h"
#include "model/musteri.h"
#include "model/arac.h"
#include "model/kiralama.h"
#include "model/odeme.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Musteri* musteri, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnKirala_clicked();
    void on_btnCikis_clicked();
    void on_btnKartKaydet_clicked();      // Kart kaydet butonu
    void on_tabWidget_currentChanged(int index);
    void on_txtArama_textChanged(const QString& arananKelime);
    void araclariYukle();
    void gecmisYukle();
    void odemeGecmisYukle();
    void kartBilgisiYukle();              // Kart sekmesini doldur

private:
    Ui::MainWindow *ui;
    Musteri* aktifMusteri;
    QList<Arac> tumAraclar;
    bool odemeEkraniGoster(int aracId, double tutar, int gun,
                            QString marka, QString model);
};

#endif // MAINWINDOW_H
