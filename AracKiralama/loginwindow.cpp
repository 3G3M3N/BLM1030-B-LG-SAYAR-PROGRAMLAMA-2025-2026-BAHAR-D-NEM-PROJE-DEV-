#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "adminpanel.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);
    setWindowTitle("Araç Kiralama - Giriş");
    DbManager::getInstance()->baglan("arac_kiralama.db");
}

LoginWindow::~LoginWindow() { delete ui; }

void LoginWindow::on_btnGiris_clicked() {
    QString email = ui->txtEmail->text().trimmed();
    QString sifre = ui->txtSifre->text();

    if (email.isEmpty() || sifre.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Email ve şifre boş olamaz!");
        return;
    }

    Kullanici* k = DbManager::getInstance()->girisYap(email, sifre);
    if (!k) {
        QMessageBox::critical(this, "Hata", "Geçersiz email veya şifre!");
        return;
    }

    if (k->getRol() == "yonetici") {
        Yonetici* y = dynamic_cast<Yonetici*>(k);
        AdminPanel* ap = new AdminPanel(y, this);
        ap->show();
    } else {
        Musteri* m = dynamic_cast<Musteri*>(k);
        MainWindow* mw = new MainWindow(m, this);
        mw->show();
    }
    this->hide();
}

void LoginWindow::on_btnKayit_clicked() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Yeni Üye Kaydı");
    dialog->setMinimumWidth(340);

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QLineEdit* txtAd    = new QLineEdit(dialog);
    QLineEdit* txtSoyad = new QLineEdit(dialog);
    QLineEdit* txtEmail = new QLineEdit(dialog);
    QLineEdit* txtSifre = new QLineEdit(dialog);
    QLineEdit* txtTc    = new QLineEdit(dialog);
    QLineEdit* txtTel   = new QLineEdit(dialog);

    txtSifre->setEchoMode(QLineEdit::Password);
    txtAd->setPlaceholderText("Adınız");
    txtSoyad->setPlaceholderText("Soyadınız");
    txtEmail->setPlaceholderText("ornek@email.com");
    txtSifre->setPlaceholderText("Şifre");
    txtTc->setPlaceholderText("11 haneli TC Kimlik No");
    txtTel->setPlaceholderText("5XXXXXXXXX (10 hane)");

    txtTc->setValidator(new QRegularExpressionValidator(
        QRegularExpression("[0-9]{0,11}"), dialog));
    txtTc->setMaxLength(11);
    txtTel->setValidator(new QRegularExpressionValidator(
        QRegularExpression("[0-9]{0,10}"), dialog));
    txtTel->setMaxLength(10);



    QPushButton* btnKaydet = new QPushButton("Kaydet", dialog);

    layout->addWidget(new QLabel("Ad:"));
    layout->addWidget(txtAd);
    layout->addWidget(new QLabel("Soyad:"));
    layout->addWidget(txtSoyad);
    layout->addWidget(new QLabel("E-posta:"));
    layout->addWidget(txtEmail);
    layout->addWidget(new QLabel("Şifre:"));
    layout->addWidget(txtSifre);
    layout->addWidget(new QLabel("TC Kimlik No:"));
    layout->addWidget(txtTc);
    layout->addWidget(new QLabel("Telefon:"));
    layout->addWidget(txtTel);
    layout->addWidget(btnKaydet);

    connect(btnKaydet, &QPushButton::clicked, [=]() {
        if (txtAd->text().trimmed().isEmpty() ||
            txtSoyad->text().trimmed().isEmpty() ||
            txtEmail->text().trimmed().isEmpty() ||
            txtSifre->text().isEmpty()) {
            QMessageBox::warning(dialog, "Uyarı", "Tüm alanları doldurun!");
            return;
        }

        // Email format kontrolü
        QRegularExpression emailRegex(
            "^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$");
        if (!emailRegex.match(txtEmail->text().trimmed()).hasMatch()) {
            QMessageBox::warning(dialog, "Geçersiz Email",
                                 "Lütfen geçerli bir email girin.\nÖrnek: kullanici@email.com");
            txtEmail->setFocus();
            return;
        }

        // TC kontrolü
        QString tc = txtTc->text().trimmed();
        if (tc.length() != 11 || tc.at(0) == '0') {
            QMessageBox::warning(dialog, "Geçersiz TC",
                                 "TC Kimlik No 11 haneli olmalı ve 0 ile başlamamalıdır!");
            txtTc->setFocus();
            return;
        }

        // Telefon kontrolü
        QString tel = txtTel->text().trimmed();
        if (tel.length() != 10 || tel.at(0) != '5') {
            QMessageBox::warning(dialog, "Geçersiz Telefon",
                                 "Telefon 10 haneli ve 5 ile başlamalıdır!");
            txtTel->setFocus();
            return;
        }

        Musteri* m = new Musteri(
            txtAd->text().trimmed(), txtSoyad->text().trimmed(),
            txtEmail->text().trimmed(), txtSifre->text(), tc, tel);

        if (DbManager::getInstance()->kullaniciKaydet(m)) {
            QMessageBox::information(dialog, "Başarılı",
                                     "Kayıt tamamlandı! Giriş yapabilirsiniz.");
            dialog->accept();
        } else {
            QMessageBox::critical(dialog, "Hata",
                                  "Kayıt başarısız! Bu email zaten kullanımda olabilir.");
        }
        delete m;
    });

    dialog->exec();
}

void LoginWindow::tekrarGoster() {
    ui->txtEmail->clear();
    ui->txtSifre->clear();
    this->show();
}
