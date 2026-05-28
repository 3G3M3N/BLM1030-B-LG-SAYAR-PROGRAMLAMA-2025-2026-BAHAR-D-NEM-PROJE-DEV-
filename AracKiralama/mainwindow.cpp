#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(Musteri* musteri, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    aktifMusteri(musteri) {
    ui->setupUi(this);
    setWindowTitle("Araç Kiralama - " +
                   musteri->getAd() + " " + musteri->getSoyad());

    ui->dateBaslangic->setMinimumDate(QDate::currentDate());
    ui->dateBitis->setMinimumDate(QDate::currentDate().addDays(1));
    ui->dateBaslangic->setDate(QDate::currentDate());
    ui->dateBitis->setDate(QDate::currentDate().addDays(1));

    connect(ui->dateBaslangic, &QDateEdit::dateChanged, [=](QDate tarih) {
        ui->dateBitis->setMinimumDate(tarih.addDays(1));
        if (ui->dateBitis->date() <= tarih)
            ui->dateBitis->setDate(tarih.addDays(1));
    });

    // Kart no formatlaması - otomatik boşluk
    connect(ui->txtKartNo, &QLineEdit::textChanged, [=](QString text) {
        QString sadece = text.remove(' ').left(16);
        QString formatli;
        for (int i = 0; i < sadece.length(); i++) {
            if (i > 0 && i % 4 == 0) formatli += ' ';
            formatli += sadece[i];
        }
        if (formatli != ui->txtKartNo->text()) {
            ui->txtKartNo->blockSignals(true);
            ui->txtKartNo->setText(formatli);
            ui->txtKartNo->blockSignals(false);
        }
    });

    // Son kullanma tarihi - otomatik /
    connect(ui->txtSonKullanma, &QLineEdit::textChanged, [=](QString text) {
        QString sadece = text.remove('/').left(4);
        QString formatli;
        for (int i = 0; i < sadece.length(); i++) {
            if (i == 2) formatli += '/';
            formatli += sadece[i];
        }
        if (formatli != ui->txtSonKullanma->text()) {
            ui->txtSonKullanma->blockSignals(true);
            ui->txtSonKullanma->setText(formatli);
            ui->txtSonKullanma->blockSignals(false);
        }
    });

    araclariYukle();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_tabWidget_currentChanged(int index) {
    if      (index == 0) araclariYukle();
    else if (index == 1) gecmisYukle();
    else if (index == 2) odemeGecmisYukle();
    else if (index == 3) kartBilgisiYukle();
}

void MainWindow::araclariYukle() {
    tumAraclar = DbManager::getInstance()->musaitAraclariGetir();
    ui->tblAraclar->setRowCount(0);
    ui->tblAraclar->setColumnCount(6);
    ui->tblAraclar->setHorizontalHeaderLabels(
        {"ID","Marka","Model","Yıl","Plaka","Günlük Fiyat"});
    for (int i = 0; i < tumAraclar.size(); i++) {
        ui->tblAraclar->insertRow(i);
        ui->tblAraclar->setItem(i,0, new QTableWidgetItem(QString::number(tumAraclar[i].getId())));
        ui->tblAraclar->setItem(i,1, new QTableWidgetItem(tumAraclar[i].getMarka()));
        ui->tblAraclar->setItem(i,2, new QTableWidgetItem(tumAraclar[i].getModel()));
        ui->tblAraclar->setItem(i,3, new QTableWidgetItem(QString::number(tumAraclar[i].getYil())));
        ui->tblAraclar->setItem(i,4, new QTableWidgetItem(tumAraclar[i].getPlaka()));
        ui->tblAraclar->setItem(i,5, new QTableWidgetItem(QString::number(tumAraclar[i].getGunlukFiyat())));
    }
}

void MainWindow::on_txtArama_textChanged(const QString& arananKelime) {
    if (arananKelime.trimmed().isEmpty()) { araclariYukle(); return; }
    QString aranan = arananKelime.toLower().trimmed();
    ui->tblAraclar->setRowCount(0);
    for (int i = 0; i < tumAraclar.size(); i++) {
        if (tumAraclar[i].getMarka().toLower().contains(aranan) ||
            tumAraclar[i].getModel().toLower().contains(aranan) ||
            tumAraclar[i].getPlaka().toLower().contains(aranan)) {
            int row = ui->tblAraclar->rowCount();
            ui->tblAraclar->insertRow(row);
            ui->tblAraclar->setItem(row,0, new QTableWidgetItem(QString::number(tumAraclar[i].getId())));
            ui->tblAraclar->setItem(row,1, new QTableWidgetItem(tumAraclar[i].getMarka()));
            ui->tblAraclar->setItem(row,2, new QTableWidgetItem(tumAraclar[i].getModel()));
            ui->tblAraclar->setItem(row,3, new QTableWidgetItem(QString::number(tumAraclar[i].getYil())));
            ui->tblAraclar->setItem(row,4, new QTableWidgetItem(tumAraclar[i].getPlaka()));
            ui->tblAraclar->setItem(row,5, new QTableWidgetItem(QString::number(tumAraclar[i].getGunlukFiyat())));
        }
    }
}

void MainWindow::gecmisYukle() {
    QList<Kiralama> gecmis =
        DbManager::getInstance()->musteriKiralamalari(aktifMusteri->getId());
    ui->tblGecmis->setRowCount(0);
    ui->tblGecmis->setColumnCount(4);
    ui->tblGecmis->setHorizontalHeaderLabels({"ID","Araç ID","Başlangıç","Durum"});
    for (int i = 0; i < gecmis.size(); i++) {
        ui->tblGecmis->insertRow(i);
        ui->tblGecmis->setItem(i,0, new QTableWidgetItem(QString::number(gecmis[i].getId())));
        ui->tblGecmis->setItem(i,1, new QTableWidgetItem(QString::number(gecmis[i].getAracId())));
        ui->tblGecmis->setItem(i,2, new QTableWidgetItem(gecmis[i].getBaslangic().toString("dd.MM.yyyy")));
        ui->tblGecmis->setItem(i,3, new QTableWidgetItem(gecmis[i].getDurum()));
    }
}

void MainWindow::odemeGecmisYukle() {
    auto odemeler = DbManager::getInstance()->musteriOdemeleri(aktifMusteri->getId());
    ui->tblOdemeler->setRowCount(0);
    ui->tblOdemeler->setColumnCount(5);
    ui->tblOdemeler->setHorizontalHeaderLabels(
        {"ID","Kiralama ID","Tarih","Tutar (TL)","Kart"});
    for (int i = 0; i < odemeler.size(); i++) {
        ui->tblOdemeler->insertRow(i);
        ui->tblOdemeler->setItem(i,0, new QTableWidgetItem(odemeler[i]["id"]));
        ui->tblOdemeler->setItem(i,1, new QTableWidgetItem(odemeler[i]["kiralama_id"]));
        QDate tarih = QDate::fromString(odemeler[i]["tarih"], "yyyy-MM-dd");
        ui->tblOdemeler->setItem(i,2, new QTableWidgetItem(tarih.toString("dd.MM.yyyy")));
        ui->tblOdemeler->setItem(i,3, new QTableWidgetItem(odemeler[i]["tutar"] + " TL"));
        ui->tblOdemeler->setItem(i,4, new QTableWidgetItem(
            "**** **** **** " + odemeler[i]["kart_son_dort"]));
    }
}

// Kart sekmesi açılınca mevcut kart bilgisini formu doldur
void MainWindow::kartBilgisiYukle() {
    QMap<QString,QString> kart =
        DbManager::getInstance()->kartGetir(aktifMusteri->getId());

    if (!kart.isEmpty()) {
        // Kart numarasını formatlı göster
        QString no = kart["kart_no"];
        QString formatli;
        for (int i = 0; i < no.length(); i++) {
            if (i > 0 && i % 4 == 0) formatli += ' ';
            formatli += no[i];
        }
        ui->txtKartNo->setText(formatli);
        ui->txtKartAd->setText(kart["kart_ad"]);
        ui->txtSonKullanma->setText(kart["son_kullanma"]);
        ui->txtCvv->clear(); // CVV güvenlik için gösterilmez
        ui->lblKartDurum->setText("Kayıtlı kartınız mevcut.");
        ui->lblKartDurum->setStyleSheet("color: #2e7d32; font-weight: bold;");
    } else {
        ui->txtKartNo->clear();
        ui->txtKartAd->clear();
        ui->txtSonKullanma->clear();
        ui->txtCvv->clear();
        ui->lblKartDurum->setText("Kayıtlı kart bulunamadı. Lütfen kart ekleyin.");
        ui->lblKartDurum->setStyleSheet("color: #c62828; font-weight: bold;");
    }
}

// Kart kaydet / güncelle
void MainWindow::on_btnKartKaydet_clicked() {
    QString kartNo = ui->txtKartNo->text().remove(' ');
    QString kartAd = ui->txtKartAd->text().trimmed();
    QString sk     = ui->txtSonKullanma->text().trimmed();
    QString cvv    = ui->txtCvv->text().trimmed();

    // Kart no kontrolü
    if (kartNo.length() != 16) {
        QMessageBox::warning(this, "Hata", "Kart numarası 16 haneli olmalıdır!");
        ui->txtKartNo->setFocus();
        return;
    }

    // Kart adı kontrolü
    if (kartAd.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Kart üzerindeki adı girin!");
        ui->txtKartAd->setFocus();
        return;
    }

    // Son kullanma tarihi kontrolü
    QRegularExpression skRegex("^(0[1-9]|1[0-2])/[0-9]{2}$");
    if (!skRegex.match(sk).hasMatch()) {
        QMessageBox::warning(this, "Hata",
                             "Son kullanma tarihi AA/YY formatında olmalıdır!\nÖrnek: 12/28");
        ui->txtSonKullanma->setFocus();
        return;
    }

    // CVV kontrolü
    if (cvv.length() != 3) {
        QMessageBox::warning(this, "Hata", "CVV 3 haneli olmalıdır!");
        ui->txtCvv->setFocus();
        return;
    }

    if (DbManager::getInstance()->kartKaydet(
            aktifMusteri->getId(), kartNo, kartAd, sk, cvv)) {
        QMessageBox::information(this, "Başarılı",
                                 "Kart bilgileri kaydedildi!");
        kartBilgisiYukle();
    } else {
        QMessageBox::critical(this, "Hata", "Kart kaydedilemedi!");
    }
}

bool MainWindow::odemeEkraniGoster(int aracId, double tutar,
                                    int gun, QString marka, QString model) {
    QMap<QString,QString> kart =
        DbManager::getInstance()->kartGetir(aktifMusteri->getId());

    if (kart.isEmpty()) {
        QMessageBox::warning(this, "Kart Bulunamadı",
                             "Hesabınıza kayıtlı kart bulunamadı!\n"
                             "Lütfen 'Kartım' sekmesinden kart ekleyin.");
        return false;
    }

    QString kartNo   = kart["kart_no"];
    QString sonDort  = kartNo.right(4);
    QString maskelenmis = "**** **** **** " + sonDort;

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Ödeme");
    dialog->setMinimumWidth(320);

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QLabel* lblArac = new QLabel(
        QString("<b>%1 %2</b> - %3 gün").arg(marka).arg(model).arg(gun));
    QLabel* lblTutar = new QLabel(
        QString("<h2 style='color:#1565c0;'>%1 TL</h2>").arg(tutar));
    lblTutar->setAlignment(Qt::AlignCenter);

    QFrame* kartFrame = new QFrame(dialog);
    kartFrame->setStyleSheet(
        "QFrame { background-color: #1565c0; border-radius: 10px; padding: 12px; }");
    QVBoxLayout* kartLayout = new QVBoxLayout(kartFrame);

    QLabel* lblKartNo = new QLabel(maskelenmis);
    QLabel* lblKartAd = new QLabel(kart["kart_ad"].toUpper());
    QLabel* lblSK     = new QLabel("Son Kullanma: " + kart["son_kullanma"]);
    lblKartNo->setStyleSheet("color: white; font-size: 16px; letter-spacing: 3px; font-weight: bold;");
    lblKartAd->setStyleSheet("color: white; font-size: 13px;");
    lblSK->setStyleSheet("color: #bbdefb; font-size: 12px;");

    kartLayout->addWidget(lblKartNo);
    kartLayout->addWidget(lblKartAd);
    kartLayout->addWidget(lblSK);

    QPushButton* btnOde   = new QPushButton("Ödemeyi Tamamla", dialog);
    QPushButton* btnIptal = new QPushButton("İptal", dialog);
    btnOde->setStyleSheet(
        "background-color: #2e7d32; color: white; font-weight: bold; "
        "padding: 10px; border-radius: 6px; font-size: 14px;");
    btnIptal->setStyleSheet(
        "background-color: #c62828; color: white; font-weight: bold; "
        "padding: 10px; border-radius: 6px;");

    layout->addWidget(lblArac);
    layout->addWidget(lblTutar);
    layout->addWidget(new QLabel("Ödeme yapılacak kart:"));
    layout->addWidget(kartFrame);
    layout->addWidget(btnOde);
    layout->addWidget(btnIptal);

    bool odemeOnaylandi = false;
    connect(btnOde,   &QPushButton::clicked, [&]() { odemeOnaylandi = true; dialog->accept(); });
    connect(btnIptal, &QPushButton::clicked, dialog, &QDialog::reject);
    dialog->exec();
    return odemeOnaylandi;
}

void MainWindow::on_btnKirala_clicked() {
    int row = ui->tblAraclar->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Uyarı", "Lütfen bir araç seçin!"); return; }

    int    aracId    = ui->tblAraclar->item(row,0)->text().toInt();
    double fiyat     = ui->tblAraclar->item(row,5)->text().toDouble();
    QString marka    = ui->tblAraclar->item(row,1)->text();
    QString model    = ui->tblAraclar->item(row,2)->text();
    QDate  baslangic = ui->dateBaslangic->date();
    QDate  bitis     = ui->dateBitis->date();

    if (baslangic >= bitis) {
        QMessageBox::warning(this, "Uyarı", "Bitiş tarihi başlangıçtan sonra olmalı!");
        return;
    }

    int gun = baslangic.daysTo(bitis);
    double toplam = gun * fiyat;

    bool odendi = odemeEkraniGoster(aracId, toplam, gun, marka, model);
    if (!odendi) return;

    Kiralama k(aktifMusteri->getId(), aracId, baslangic, bitis, fiyat);
    if (DbManager::getInstance()->kiralamaOlustur(k)) {
        QMap<QString,QString> kart =
            DbManager::getInstance()->kartGetir(aktifMusteri->getId());
        Odeme odeme(k.getId(), aktifMusteri->getId(), toplam, kart["kart_no"].right(4));
        DbManager::getInstance()->odemeKaydet(odeme);
        QMessageBox::information(this, "Ödeme Başarılı",
                                 QString("Ödeme tamamlandı!\nAraç: %1 %2\nSüre: %3 gün\nTutar: %4 TL")
                                     .arg(marka).arg(model).arg(gun).arg(toplam));
        araclariYukle();
    } else {
        QMessageBox::critical(this, "Hata", "Kiralama gerçekleşemedi!");
    }
}

void MainWindow::on_btnCikis_clicked() {
    QMessageBox cikisBox(this);
    cikisBox.setWindowTitle("Çıkış");
    cikisBox.setText("Hesabınızdan çıkmak istiyor musunuz?");
    QPushButton* evetBtn = cikisBox.addButton("Evet", QMessageBox::YesRole);
    cikisBox.addButton("Hayır", QMessageBox::NoRole);
    cikisBox.exec();
    if (cikisBox.clickedButton() != evetBtn) return;

    LoginWindow* login = qobject_cast<LoginWindow*>(this->parent());
    if (login) login->tekrarGoster();
    delete aktifMusteri;
    this->close();
}
