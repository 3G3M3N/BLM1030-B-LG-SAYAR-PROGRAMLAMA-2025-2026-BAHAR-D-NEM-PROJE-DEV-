#include "dbmanager.h"

DbManager* DbManager::instance = nullptr;

DbManager::DbManager() {}

DbManager* DbManager::getInstance() {
    if (!instance) instance = new DbManager();
    return instance;
}

bool DbManager::baglan(const QString& dbYolu) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbYolu);
    if (!db.open()) {
        qDebug() << "DB Hatasi:" << db.lastError().text();
        return false;
    }
    tablolarOlustur();
    return true;
}

void DbManager::tablolarOlustur() {
    QSqlQuery q;

    q.exec(R"(CREATE TABLE IF NOT EXISTS araclar (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        marka TEXT, model TEXT, yil INTEGER,
        plaka TEXT UNIQUE, renk TEXT, kategori TEXT,
        gunluk_fiyat REAL, musait INTEGER DEFAULT 1,
        km INTEGER DEFAULT 0, durum TEXT DEFAULT 'Aktif'))");

    q.exec(R"(CREATE TABLE IF NOT EXISTS kullanicilar (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        ad TEXT, soyad TEXT, email TEXT UNIQUE,
        sifre TEXT, rol TEXT, tc_kimlik TEXT,
        telefon TEXT, adres TEXT, yetki INTEGER DEFAULT 1))");

    q.exec(R"(CREATE TABLE IF NOT EXISTS kiralamalar (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        musteri_id INTEGER, arac_id INTEGER,
        baslangic TEXT, bitis TEXT,
        toplam_tutar REAL, durum TEXT DEFAULT 'Aktif',
        notlar TEXT))");

    q.exec(R"(CREATE TABLE IF NOT EXISTS bakim_kayitlari (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        arac_id INTEGER, tarih TEXT,
        aciklama TEXT, maliyet REAL))");

    q.exec(R"(CREATE TABLE IF NOT EXISTS kartlar (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        musteri_id INTEGER UNIQUE,
        kart_no TEXT, kart_ad TEXT,
        son_kullanma TEXT, cvv TEXT))");

    q.exec(R"(CREATE TABLE IF NOT EXISTS odemeler (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        kiralama_id INTEGER, musteri_id INTEGER,
        tutar REAL, tarih TEXT,
        durum TEXT DEFAULT 'Tamamlandi',
        kart_son_dort TEXT))");

    // varsayilan admin hesabi
    q.exec(R"(INSERT OR IGNORE INTO kullanicilar
        (ad,soyad,email,sifre,rol,yetki)
        VALUES('Admin','User','admin@rentacar.com',
               'admin123','yonetici',2))");
}

bool DbManager::aracEkle(Arac& arac) {
    QSqlQuery q;
    q.prepare(R"(INSERT INTO araclar
        (marka,model,yil,plaka,renk,kategori,gunluk_fiyat)
        VALUES(:marka,:model,:yil,:plaka,:renk,:kategori,:fiyat))");
    q.bindValue(":marka",    arac.getMarka());
    q.bindValue(":model",    arac.getModel());
    q.bindValue(":yil",      arac.getYil());
    q.bindValue(":plaka",    arac.getPlaka());
    q.bindValue(":renk",     arac.getRenk());
    q.bindValue(":kategori", arac.getKategori());
    q.bindValue(":fiyat",    arac.getGunlukFiyat());
    if (q.exec()) {
        arac.setId(q.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool DbManager::aracSil(int id) {
    QSqlQuery q;
    q.prepare("DELETE FROM araclar WHERE id=:id");
    q.bindValue(":id", id);
    return q.exec();
}

bool DbManager::aracGuncelle(const Arac& arac) {
    QSqlQuery q;
    q.prepare(R"(UPDATE araclar SET
        gunluk_fiyat=:fiyat, musait=:musait,
        km=:km, durum=:durum WHERE id=:id)");
    q.bindValue(":fiyat",  arac.getGunlukFiyat());
    q.bindValue(":musait", arac.isMusait() ? 1 : 0);
    q.bindValue(":km",     arac.getKm());
    q.bindValue(":durum",  arac.getDurum());
    q.bindValue(":id",     arac.getId());
    return q.exec();
}

QList<Arac> DbManager::musaitAraclariGetir() {
    QList<Arac> liste;
    QSqlQuery q("SELECT * FROM araclar WHERE musait=1");
    while (q.next()) {
        Arac a(q.value("marka").toString(),
               q.value("model").toString(),
               q.value("yil").toInt(),
               q.value("plaka").toString(),
               q.value("renk").toString(),
               q.value("kategori").toString(),
               q.value("gunluk_fiyat").toDouble());
        a.setId(q.value("id").toInt());
        a.setKm(q.value("km").toInt());
        a.setDurum(q.value("durum").toString());
        liste.append(a);
    }
    return liste;
}

QList<Arac> DbManager::tumAraclariGetir() {
    QList<Arac> liste;
    QSqlQuery q("SELECT * FROM araclar");
    while (q.next()) {
        Arac a(q.value("marka").toString(),
               q.value("model").toString(),
               q.value("yil").toInt(),
               q.value("plaka").toString(),
               q.value("renk").toString(),
               q.value("kategori").toString(),
               q.value("gunluk_fiyat").toDouble());
        a.setId(q.value("id").toInt());
        a.setMusait(q.value("musait").toInt() == 1);
        a.setKm(q.value("km").toInt());
        a.setDurum(q.value("durum").toString());
        liste.append(a);
    }
    return liste;
}

bool DbManager::kullaniciKaydet(Kullanici* k) {
    QSqlQuery q;
    if (k->getRol() == "musteri") {
        Musteri* m = dynamic_cast<Musteri*>(k);
        q.prepare(R"(INSERT INTO kullanicilar
            (ad,soyad,email,sifre,rol,tc_kimlik,telefon)
            VALUES(:ad,:soyad,:email,:sifre,'musteri',:tc,:tel))");
        q.bindValue(":tc",  m->getTcKimlik());
        q.bindValue(":tel", m->getTelefon());
    } else {
        q.prepare(R"(INSERT INTO kullanicilar
            (ad,soyad,email,sifre,rol,yetki)
            VALUES(:ad,:soyad,:email,:sifre,'yonetici',1))");
    }
    q.bindValue(":ad",    k->getAd());
    q.bindValue(":soyad", k->getSoyad());
    q.bindValue(":email", k->getEmail());
    q.bindValue(":sifre", k->getSifre());
    if (q.exec()) {
        k->setId(q.lastInsertId().toInt());
        return true;
    }
    return false;
}

Kullanici* DbManager::girisYap(const QString& email,
                               const QString& sifre) {
    QSqlQuery q;
    q.prepare("SELECT * FROM kullanicilar WHERE email=:e AND sifre=:s");
    q.bindValue(":e", email);
    q.bindValue(":s", sifre);
    if (!q.exec() || !q.next()) return nullptr;

    QString rol = q.value("rol").toString();
    Kullanici* k = nullptr;

    if (rol == "musteri") {
        Musteri* m = new Musteri(
            q.value("ad").toString(),
            q.value("soyad").toString(),
            q.value("email").toString(),
            q.value("sifre").toString(),
            q.value("tc_kimlik").toString(),
            q.value("telefon").toString());
        k = m;
    } else {
        Yonetici* y = new Yonetici(
            q.value("ad").toString(),
            q.value("soyad").toString(),
            q.value("email").toString(),
            q.value("sifre").toString(),
            q.value("yetki").toInt());
        k = y;
    }
    k->setId(q.value("id").toInt());
    return k;
}

QList<Musteri> DbManager::tumMusterileriGetir() {
    QList<Musteri> liste;
    QSqlQuery q("SELECT * FROM kullanicilar WHERE rol='musteri'");
    while (q.next()) {
        Musteri m(q.value("ad").toString(),
                  q.value("soyad").toString(),
                  q.value("email").toString(),
                  q.value("sifre").toString(),
                  q.value("tc_kimlik").toString(),
                  q.value("telefon").toString());
        m.setId(q.value("id").toInt());
        liste.append(m);
    }
    return liste;
}

bool DbManager::kiralamaOlustur(Kiralama& k) {
    QSqlQuery q;
    q.prepare(R"(INSERT INTO kiralamalar
        (musteri_id,arac_id,baslangic,bitis,toplam_tutar)
        VALUES(:mid,:aid,:bas,:bit,:tutar))");
    q.bindValue(":mid",   k.getMusteriId());
    q.bindValue(":aid",   k.getAracId());
    q.bindValue(":bas",   k.getBaslangic().toString("yyyy-MM-dd"));
    q.bindValue(":bit",   k.getBitis().toString("yyyy-MM-dd"));
    q.bindValue(":tutar", k.getToplamTutar());
    if (!q.exec()) return false;
    k.setId(q.lastInsertId().toInt());

    QSqlQuery q2;
    q2.prepare("UPDATE araclar SET musait=0,durum='Kirada' WHERE id=:id");
    q2.bindValue(":id", k.getAracId());
    q2.exec();
    return true;
}

bool DbManager::kiralamaKapat(int kiralamaId) {
    QSqlQuery q;
    q.prepare("SELECT arac_id FROM kiralamalar WHERE id=:id");
    q.bindValue(":id", kiralamaId);
    if (!q.exec() || !q.next()) return false;
    int aracId = q.value("arac_id").toInt();

    QSqlQuery q2;
    q2.prepare("UPDATE kiralamalar SET durum='Tamamlandi' WHERE id=:id");
    q2.bindValue(":id", kiralamaId);
    q2.exec();

    QSqlQuery q3;
    q3.prepare("UPDATE araclar SET musait=1,durum='Aktif' WHERE id=:id");
    q3.bindValue(":id", aracId);
    q3.exec();
    return true;
}

QList<Kiralama> DbManager::musteriKiralamalari(int musteriId) {
    QList<Kiralama> liste;
    QSqlQuery q;
    q.prepare("SELECT * FROM kiralamalar WHERE musteri_id=:mid");
    q.bindValue(":mid", musteriId);
    q.exec();
    while (q.next()) {
        Kiralama k(q.value("musteri_id").toInt(),
                   q.value("arac_id").toInt(),
                   QDate::fromString(q.value("baslangic").toString(),"yyyy-MM-dd"),
                   QDate::fromString(q.value("bitis").toString(),"yyyy-MM-dd"),
                   0);
        k.setId(q.value("id").toInt());
        k.setDurum(q.value("durum").toString());
        liste.append(k);
    }
    return liste;
}

QList<Kiralama> DbManager::tumKiralalar() {
    QList<Kiralama> liste;
    QSqlQuery q("SELECT * FROM kiralamalar ORDER BY id DESC");
    while (q.next()) {
        Kiralama k(q.value("musteri_id").toInt(),
                   q.value("arac_id").toInt(),
                   QDate::fromString(q.value("baslangic").toString(),"yyyy-MM-dd"),
                   QDate::fromString(q.value("bitis").toString(),"yyyy-MM-dd"),
                   0);
        k.setId(q.value("id").toInt());
        k.setDurum(q.value("durum").toString());
        liste.append(k);
    }
    return liste;
}

bool DbManager::bakimKaydet(int aracId, QDate tarih,
                            QString aciklama, double maliyet) {
    QSqlQuery q;
    q.prepare(R"(INSERT INTO bakim_kayitlari
        (arac_id,tarih,aciklama,maliyet)
        VALUES(:id,:tarih,:aciklama,:maliyet))");
    q.bindValue(":id",       aracId);
    q.bindValue(":tarih",    tarih.toString("yyyy-MM-dd"));
    q.bindValue(":aciklama", aciklama);
    q.bindValue(":maliyet",  maliyet);
    if (q.exec()) {
        QSqlQuery q2;
        q2.prepare("UPDATE araclar SET durum='Bakimda',musait=0 WHERE id=:id");
        q2.bindValue(":id", aracId);
        q2.exec();
        return true;
    }
    return false;
}

bool DbManager::bakimdenCikar(int aracId) {
    QSqlQuery q;
    q.prepare("UPDATE araclar SET durum='Aktif', musait=1 WHERE id=:id");
    q.bindValue(":id", aracId);
    return q.exec();
}

QList<QMap<QString,QString>> DbManager::tumBakimlariGetir() {
    QList<QMap<QString,QString>> liste;
    QSqlQuery q;
    q.exec(R"(SELECT b.id, b.tarih, b.aciklama, b.maliyet,
              a.marka, a.model, a.plaka
              FROM bakim_kayitlari b
              LEFT JOIN araclar a ON b.arac_id = a.id
              ORDER BY b.id DESC)");
    while (q.next()) {
        QMap<QString,QString> kayit;
        kayit["id"]       = q.value("id").toString();
        kayit["tarih"]    = q.value("tarih").toString();
        kayit["aciklama"] = q.value("aciklama").toString();
        kayit["maliyet"]  = q.value("maliyet").toString();
        kayit["marka"]    = q.value("marka").toString();
        kayit["model"]    = q.value("model").toString();
        kayit["plaka"]    = q.value("plaka").toString();
        liste.append(kayit);
    }
    return liste;
}

bool DbManager::kartKaydet(int musteriId, QString kartNo,
                            QString kartAd, QString sonKullanma,
                            QString cvv) {
    QSqlQuery q;
    q.prepare("DELETE FROM kartlar WHERE musteri_id=:mid");
    q.bindValue(":mid", musteriId);
    q.exec();

    q.prepare(R"(INSERT INTO kartlar
        (musteri_id, kart_no, kart_ad, son_kullanma, cvv)
        VALUES(:mid,:no,:ad,:sk,:cvv))");
    q.bindValue(":mid", musteriId);
    q.bindValue(":no",  kartNo);
    q.bindValue(":ad",  kartAd);
    q.bindValue(":sk",  sonKullanma);
    q.bindValue(":cvv", cvv);
    return q.exec();
}

QMap<QString,QString> DbManager::kartGetir(int musteriId) {
    QMap<QString,QString> kart;
    QSqlQuery q;
    q.prepare("SELECT * FROM kartlar WHERE musteri_id=:mid");
    q.bindValue(":mid", musteriId);
    q.exec();
    if (q.next()) {
        kart["kart_no"]      = q.value("kart_no").toString();
        kart["kart_ad"]      = q.value("kart_ad").toString();
        kart["son_kullanma"] = q.value("son_kullanma").toString();
        kart["cvv"]          = q.value("cvv").toString();
    }
    return kart;
}

bool DbManager::odemeKaydet(Odeme& odeme) {
    QSqlQuery q;
    q.prepare(R"(INSERT INTO odemeler
        (kiralama_id, musteri_id, tutar, tarih, durum, kart_son_dort)
        VALUES(:kid,:mid,:tutar,:tarih,:durum,:ksd))");
    q.bindValue(":kid",   odeme.getKiralamaId());
    q.bindValue(":mid",   odeme.getMusteriId());
    q.bindValue(":tutar", odeme.getTutar());
    q.bindValue(":tarih", odeme.getTarih().toString("yyyy-MM-dd"));
    q.bindValue(":durum", odeme.getDurum());
    q.bindValue(":ksd",   odeme.getKartSonDort());
    if (q.exec()) {
        odeme.setId(q.lastInsertId().toInt());
        return true;
    }
    return false;
}

QList<QMap<QString,QString>> DbManager::tumOdemeleriGetir() {
    QList<QMap<QString,QString>> liste;
    QSqlQuery q;
    q.exec(R"(SELECT o.id, o.tarih, o.tutar, o.durum,
              o.kart_son_dort, o.kiralama_id,
              k.ad, k.soyad
              FROM odemeler o
              LEFT JOIN kullanicilar k ON o.musteri_id = k.id
              ORDER BY o.id DESC)");
    while (q.next()) {
        QMap<QString,QString> kayit;
        kayit["id"]            = q.value("id").toString();
        kayit["tarih"]         = q.value("tarih").toString();
        kayit["tutar"]         = q.value("tutar").toString();
        kayit["durum"]         = q.value("durum").toString();
        kayit["kart_son_dort"] = q.value("kart_son_dort").toString();
        kayit["kiralama_id"]   = q.value("kiralama_id").toString();
        kayit["musteri"]       = q.value("ad").toString() + " " +
                                 q.value("soyad").toString();
        liste.append(kayit);
    }
    return liste;
}

QList<QMap<QString,QString>> DbManager::musteriOdemeleri(int musteriId) {
    QList<QMap<QString,QString>> liste;
    QSqlQuery q;
    q.prepare("SELECT * FROM odemeler WHERE musteri_id=:mid ORDER BY id DESC");
    q.bindValue(":mid", musteriId);
    q.exec();
    while (q.next()) {
        QMap<QString,QString> kayit;
        kayit["id"]            = q.value("id").toString();
        kayit["tarih"]         = q.value("tarih").toString();
        kayit["tutar"]         = q.value("tutar").toString();
        kayit["durum"]         = q.value("durum").toString();
        kayit["kart_son_dort"] = q.value("kart_son_dort").toString();
        kayit["kiralama_id"]   = q.value("kiralama_id").toString();
        liste.append(kayit);
    }
    return liste;
}
