#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "database/dbmanager.h"
#include "model/musteri.h"
#include "model/yonetici.h"

namespace Ui { class LoginWindow; }

class LoginWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    // Çıkış yapınca tekrar giriş ekranını göster
    void tekrarGoster();

private slots:
    void on_btnGiris_clicked();
    void on_btnKayit_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
