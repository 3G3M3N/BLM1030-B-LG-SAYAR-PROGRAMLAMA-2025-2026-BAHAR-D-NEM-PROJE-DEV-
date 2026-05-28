#include "loginwindow.h"
#include "adminpanel.h"
#include "ui_adminpanel.h"

AdminPanel::AdminPanel(Yonetici* yonetici, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AdminPanel),
    aktifYonetici(yonetici) {
    ui->setupUi(this);
    setWindowTitle("Yönetici Paneli - " +
                   yonetici->getAd() + " " +
                   yonetici->getSoyad());
    araclariYukle();
    kiralamalariYukle();
    musterileriYukle();
    bakimlariYukle();
    odemeleriYukle(); // Ödeme sekmesini yükle
}

AdminPanel::~AdminPanel() { delete ui; }

void AdminPanel::araclariYukle() {
    tumAraclar = DbManager::getInstance()->tumAraclariGetir();

    ui->tblAraclar->setRowCount(0);
    ui->tblAraclar->setColumnCount(7);
    ui->tblAraclar->setHorizontalHeaderLabels(
        {"ID","Marka","Model","Yıl","Plaka","Fiyat","Durum"});

    for (int i = 0; i < tumAraclar.size(); i++) {
        ui->tblAraclar->insertRow(i);
        ui->tblAraclar->setItem(i,0, new QTableWidgetItem(QString::number(tumAraclar[i].getId())));
        ui->tblAraclar->setItem(i,1, new QTableWidgetItem(tumAraclar[i].getMarka()));
        ui->tblAraclar->setItem(i,2, new QTableWidgetItem(tumAraclar[i].getModel()));
        ui->tblAraclar->setItem(i,3, new QTableWidgetItem(QString::number(tumAraclar[i].getYil())));
        ui->tblAraclar->setItem(i,4, new QTableWidgetItem(tumAraclar[i].getPlaka()));
        ui->tblAraclar->setItem(i,5, new QTableWidgetItem(QString::number(tumAraclar[i].getGunlukFiyat())));
        ui->tblAraclar->setItem(i,6, new QTableWidgetItem(tumAraclar[i].getDurum()));
    }
}

void AdminPanel::on_txtAracArama_textChanged(const QString& arananKelime) {
    if (arananKelime.trimmed().isEmpty()) { araclariYukle(); return; }

    QString aranan = arananKelime.toLower().trimmed();
    ui->tblAraclar->setRowCount(0);

    for (int i = 0; i < tumAraclar.size(); i++) {
        if (tumAraclar[i].getMarka().toLower().contains(aranan) ||
            tumAraclar[i].getModel().toLower().contains(aranan) ||
            tumAraclar[i].getPlaka().toLower().contains(aranan) ||
            tumAraclar[i].getDurum().toLower().contains(aranan)) {
            int row = ui->tblAraclar->rowCount();
            ui->tblAraclar->insertRow(row);
            ui->tblAraclar->setItem(row,0, new QTableWidgetItem(QString::number(tumAraclar[i].getId())));
            ui->tblAraclar->setItem(row,1, new QTableWidgetItem(tumAraclar[i].getMarka()));
            ui->tblAraclar->setItem(row,2, new QTableWidgetItem(tumAraclar[i].getModel()));
            ui->tblAraclar->setItem(row,3, new QTableWidgetItem(QString::number(tumAraclar[i].getYil())));
            ui->tblAraclar->setItem(row,4, new QTableWidgetItem(tumAraclar[i].getPlaka()));
            ui->tblAraclar->setItem(row,5, new QTableWidgetItem(QString::number(tumAraclar[i].getGunlukFiyat())));
            ui->tblAraclar->setItem(row,6, new QTableWidgetItem(tumAraclar[i].getDurum()));
        }
    }
}

void AdminPanel::kiralamalariYukle() {
    QList<Kiralama> kiralamalar = DbManager::getInstance()->tumKiralalar();

    ui->tblKiralamalar->setRowCount(0);
    ui->tblKiralamalar->setColumnCount(5);
    ui->tblKiralamalar->setHorizontalHeaderLabels(
        {"ID","Müşteri ID","Araç ID","Başlangıç","Durum"});

    for (int i = 0; i < kiralamalar.size(); i++) {
        ui->tblKiralamalar->insertRow(i);
        ui->tblKiralamalar->setItem(i,0, new QTableWidgetItem(QString::number(kiralamalar[i].getId())));
        ui->tblKiralamalar->setItem(i,1, new QTableWidgetItem(QString::number(kiralamalar[i].getMusteriId())));
        ui->tblKiralamalar->setItem(i,2, new QTableWidgetItem(QString::number(kiralamalar[i].getAracId())));
        ui->tblKiralamalar->setItem(i,3, new QTableWidgetItem(kiralamalar[i].getBaslangic().toString("dd.MM.yyyy")));
        ui->tblKiralamalar->setItem(i,4, new QTableWidgetItem(kiralamalar[i].getDurum()));
    }
}

void AdminPanel::musterileriYukle() {
    QList<Musteri> musteriler = DbManager::getInstance()->tumMusterileriGetir();

    ui->tblMusteriler->setRowCount(0);
    ui->tblMusteriler->setColumnCount(4);
    ui->tblMusteriler->setHorizontalHeaderLabels({"ID","Ad","Soyad","Telefon"});

    for (int i = 0; i < musteriler.size(); i++) {
        ui->tblMusteriler->insertRow(i);
        ui->tblMusteriler->setItem(i,0, new QTableWidgetItem(QString::number(musteriler[i].getId())));
        ui->tblMusteriler->setItem(i,1, new QTableWidgetItem(musteriler[i].getAd()));
        ui->tblMusteriler->setItem(i,2, new QTableWidgetItem(musteriler[i].getSoyad()));
        ui->tblMusteriler->setItem(i,3, new QTableWidgetItem(musteriler[i].getTelefon()));
    }
}

// Bakım kayıtlarını tabloya yükle - araç bilgisiyle birlikte göster
void AdminPanel::bakimlariYukle() {
    auto bakimlar = DbManager::getInstance()->tumBakimlariGetir();

    ui->tblBakimlar->setRowCount(0);
    ui->tblBakimlar->setColumnCount(6);
    ui->tblBakimlar->setHorizontalHeaderLabels(
        {"ID","Araç","Plaka","Tarih","Açıklama","Maliyet (TL)"});

    for (int i = 0; i < bakimlar.size(); i++) {
        ui->tblBakimlar->insertRow(i);
        ui->tblBakimlar->setItem(i,0, new QTableWidgetItem(bakimlar[i]["id"]));
        ui->tblBakimlar->setItem(i,1, new QTableWidgetItem(
            bakimlar[i]["marka"] + " " + bakimlar[i]["model"]));
        ui->tblBakimlar->setItem(i,2, new QTableWidgetItem(bakimlar[i]["plaka"]));
        // Tarihi dd.MM.yyyy formatına çevir
        QDate tarih = QDate::fromString(bakimlar[i]["tarih"], "yyyy-MM-dd");
        ui->tblBakimlar->setItem(i,3, new QTableWidgetItem(tarih.toString("dd.MM.yyyy")));
        ui->tblBakimlar->setItem(i,4, new QTableWidgetItem(bakimlar[i]["aciklama"]));
        ui->tblBakimlar->setItem(i,5, new QTableWidgetItem(bakimlar[i]["maliyet"]));
    }

    // Açıklama sütunu geniş olsun
    ui->tblBakimlar->horizontalHeader()->setSectionResizeMode(
        4, QHeaderView::Stretch);
}

void AdminPanel::on_btnAracEkle_clicked() {
    QString marka    = ui->txtMarka->text().trimmed();
    QString model    = ui->txtModel->text().trimmed();
    QString plaka    = ui->txtPlaka->text().trimmed();
    QString renk     = ui->txtRenk->text().trimmed();
    QString kategori = ui->cmbKategori->currentText();
    int     yil      = ui->spinYil->value();
    double  fiyat    = ui->spinFiyat->value();

    if (marka.isEmpty() || model.isEmpty() || plaka.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Marka, model ve plaka boş olamaz!");
        return;
    }

    Arac yeniArac(marka, model, yil, plaka, renk, kategori, fiyat);
    if (DbManager::getInstance()->aracEkle(yeniArac)) {
        QMessageBox::information(this, "Başarılı", "Araç eklendi!");
        araclariYukle();
    } else {
        QMessageBox::critical(this, "Hata", "Araç eklenemedi!");
    }
}

void AdminPanel::on_btnAracSil_clicked() {
    int row = ui->tblAraclar->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Uyarı", "Lütfen bir araç seçin!"); return; }
    int id = ui->tblAraclar->item(row, 0)->text().toInt();
    if (DbManager::getInstance()->aracSil(id)) {
        QMessageBox::information(this, "Başarılı", "Araç silindi!");
        araclariYukle();
    }
}

void AdminPanel::on_btnBakimEkle_clicked() {
    int row = ui->tblAraclar->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Uyarı", "Lütfen bir araç seçin!"); return; }

    int     aracId   = ui->tblAraclar->item(row,0)->text().toInt();
    QString aciklama = ui->txtBakimAciklama->text();
    double  maliyet  = ui->spinBakimMaliyet->value();

    if (DbManager::getInstance()->bakimKaydet(
            aracId, QDate::currentDate(), aciklama, maliyet)) {
        QMessageBox::information(this, "Başarılı", "Bakım kaydedildi!");
        araclariYukle();
        bakimlariYukle(); // Bakım tablosunu da güncelle
    }
}

void AdminPanel::on_btnBakimdenCikar_clicked() {
    int row = ui->tblAraclar->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Uyarı", "Lütfen bir araç seçin!"); return; }

    QString durum = ui->tblAraclar->item(row, 6)->text();
    if (durum != "Bakımda") {
        QMessageBox::warning(this, "Uyarı", "Seçili araç bakımda değil!");
        return;
    }

    int aracId = ui->tblAraclar->item(row, 0)->text().toInt();
    if (DbManager::getInstance()->bakimdenCikar(aracId)) {
        QMessageBox::information(this, "Başarılı",
                                 "Araç bakımdan çıkarıldı, artık kiralanabilir!");
        araclariYukle();
    } else {
        QMessageBox::critical(this, "Hata", "İşlem başarısız!");
    }
}

void AdminPanel::on_btnKiralamaKapat_clicked() {
    int row = ui->tblKiralamalar->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Uyarı", "Lütfen bir kiralama seçin!"); return; }

    int id = ui->tblKiralamalar->item(row,0)->text().toInt();
    if (DbManager::getInstance()->kiralamaKapat(id)) {
        QMessageBox::information(this, "Başarılı", "Kiralama kapatıldı!");
        kiralamalariYukle();
        araclariYukle();
    }
}

// Yönetici çıkış - giriş ekranına dön
void AdminPanel::on_btnCikis_clicked() {
    QMessageBox cikisBox(this);
    cikisBox.setWindowTitle("Çıkış");
    cikisBox.setText("Hesabınızdan çıkmak istiyor musunuz?");
    QPushButton* evetBtn = cikisBox.addButton("Evet", QMessageBox::YesRole);
    cikisBox.addButton("Hayır", QMessageBox::NoRole);
    cikisBox.exec();
    if (cikisBox.clickedButton() != evetBtn) return;

    LoginWindow* login = qobject_cast<LoginWindow*>(this->parent());
    if (login) login->tekrarGoster();
    delete aktifYonetici;
    this->close();
}

// Tüm ödemeleri tabloya yükle
void AdminPanel::odemeleriYukle() {
    auto odemeler = DbManager::getInstance()->tumOdemeleriGetir();

    ui->tblOdemeler->setRowCount(0);
    ui->tblOdemeler->setColumnCount(6);
    ui->tblOdemeler->setHorizontalHeaderLabels(
        {"ID","Müşteri","Kiralama ID","Tarih","Tutar (TL)","Kart"});

    for (int i = 0; i < odemeler.size(); i++) {
        ui->tblOdemeler->insertRow(i);
        ui->tblOdemeler->setItem(i,0, new QTableWidgetItem(odemeler[i]["id"]));
        ui->tblOdemeler->setItem(i,1, new QTableWidgetItem(odemeler[i]["musteri"]));
        ui->tblOdemeler->setItem(i,2, new QTableWidgetItem(odemeler[i]["kiralama_id"]));
        QDate tarih = QDate::fromString(odemeler[i]["tarih"], "yyyy-MM-dd");
        ui->tblOdemeler->setItem(i,3, new QTableWidgetItem(tarih.toString("dd.MM.yyyy")));
        ui->tblOdemeler->setItem(i,4, new QTableWidgetItem(odemeler[i]["tutar"] + " TL"));
        ui->tblOdemeler->setItem(i,5, new QTableWidgetItem(
            "**** **** **** " + odemeler[i]["kart_son_dort"]));
    }
}
