// Qt uygulama giriş noktası
#include <QApplication>
#include "loginwindow.h"

int main(int argc, char *argv[]) {
    // QApplication nesnesi - Qt olay döngüsü için gerekli
    QApplication app(argc, argv);

    // Uygulama meta bilgileri
    app.setApplicationName("Araç Kiralama Sistemi");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Marmara Üniversitesi");

    // İlk pencereyi oluştur ve göster
    LoginWindow loginPencere;
    loginPencere.show();

    // Qt olay döngüsünü başlat
    return app.exec();
}
