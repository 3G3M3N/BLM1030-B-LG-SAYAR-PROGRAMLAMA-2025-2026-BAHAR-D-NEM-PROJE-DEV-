QT += core gui sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    adminpanel.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    model/arac.cpp \
    model/kullanici.cpp \
    model/musteri.cpp \
    model/yonetici.cpp \
    model/kiralama.cpp \
    database/dbmanager.cpp \
    model/odeme.cpp

HEADERS += \
    adminpanel.h \
    loginwindow.h \
    mainwindow.h \
    model/arac.h \
    model/kullanici.h \
    model/musteri.h \
    model/yonetici.h \
    model/kiralama.h \
    database/dbmanager.h \
    model/odeme.h

FORMS += \
    adminpanel.ui \
    loginwindow.ui \
    mainwindow.ui