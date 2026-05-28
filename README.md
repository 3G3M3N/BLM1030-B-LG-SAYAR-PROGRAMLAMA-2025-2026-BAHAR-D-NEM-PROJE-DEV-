# Araç Kiralama Uygulaması (Car Rental System)

Bu proje, C++ ve Qt Framework kullanılarak geliştirilmiş bir **Araç Kiralama Uygulamasıdır**. SQL veritabanı entegrasyonu ile araçların, müşterilerin, kiralamaların ve ödemelerin yönetimini sağlar.

## Özellikler
- **Giriş Paneli (Login Window):** Yönetici ve müşteri giriş yetkilendirmesi.
- **Yönetici Paneli (Admin Panel):**
  - Araç ekleme, silme ve güncelleme işlemleri.
  - Müşteri kayıtlarının yönetimi.
  - Kiralama geçmişi ve detaylarının takibi.
- **Kullanıcı/Müşteri Arayüzü:** Araç listeleme, kiralama talebi oluşturma ve ödeme adımları.
- **Veritabanı Entegrasyonu:** SQL (SQLite) tabanlı veri saklama yapısı.

## Proje Yapısı
- `AracKiralama.pro`: Qt Proje yapılandırma dosyası.
- `main.cpp`: Uygulamanın giriş noktası.
- `mainwindow.h/cpp/ui`: Ana kullanıcı arayüzü ve kontrolleri.
- `adminpanel.h/cpp/ui`: Yönetici kontrol paneli.
- `loginwindow.h/cpp/ui`: Giriş penceresi.
- `database/`: Veritabanı yönetim sınıfları (`dbmanager`).
- `model/`: Projenin veri modelleri (Araç, Müşteri, Kullanıcı, Yönetici, Kiralama, Ödeme).
- `rapor.pdf`: Proje raporu.
- `uml_class_diagram.jpg`: Projenin UML Sınıf Diyagramı.

## Kurulum ve Çalıştırma
1. Bilgisayarınızda **Qt Creator** ve uygun bir derleyicinin (örn. MinGW) kurulu olduğundan emin olun.
2. `AracKiralama.pro` dosyasını Qt Creator ile açın.
3. Projeyi derleyin (Build) ve çalıştırın (Run).

Not: Proje kaynak kodları AracKiralama klasörünün içindedir. 👨‍💻
