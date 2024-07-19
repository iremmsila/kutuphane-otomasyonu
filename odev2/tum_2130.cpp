
//İREM SILA SARIKAYA 21100011030

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kitap_islemleri.h"
#include "odunc_alma.h"
#include "odunc_teslim_etme.h"
#include "uye_islemleri.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.setDatabaseName("C:/Users/Lenovo/Documents/odev2/kutuphane.db");
    if(!db.open())  //statüs bar
    {
        ui->statusbar->showMessage("Veri tabanına bağlanılamadı!");
    }
    else
    {
        ui->statusbar->showMessage("Veri tabanına bağlanıldı!");
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_kitap_clicked()
{
    kitap_islemleri *kitap = new kitap_islemleri(db);
    kitap->show();
}


void MainWindow::on_pushButton_al_clicked()
{
    odunc_alma *odunc = new odunc_alma(db);
    odunc->show();
}



void MainWindow::on_pushButton_uye_clicked()
{
    uye_islemleri *uye = new uye_islemleri(db);
    uye->show();
}


void MainWindow::on_pushButton_ver_clicked()
{
    odunc_teslim_etme *teslim = new odunc_teslim_etme(db);
    teslim->show();
}




//İREM SILA SARIKAYA 21100011030

#include "kitap_islemleri.h"
#include "ui_kitap_islemleri.h"

kitap_islemleri::kitap_islemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kitap_islemleri)
{
    ui->setupUi(this);
    sorgu = new QSqlQuery(db);
    listele();
}

kitap_islemleri::~kitap_islemleri()
{
    delete ui;
}

void kitap_islemleri::listele()
{
    sorgu->prepare("select * from kitap");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap işlemleri tablosu çekilirken bir hata ile karşılaşıldı","Ok");
        return;
    }

    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_kitaplar->setModel(model);

}

void kitap_islemleri::on_tableView_kitaplar_clicked(const QModelIndex &index)
{
    ui->lineEdit_no->setText(model->index(index.row(),0).data().toString());
    ui->lineEdit_ad->setText(model->index(index.row(),1).data().toString());
    ui->lineEdit_stok->setText(model->index(index.row(),2).data().toString());

    QString kitap = model->index(index.row(),0).data().toString();
    int kitap_no = kitap.toInt();
    sorgu->prepare("select * from odunc_alinan where kitap_no=?");
    sorgu->addBindValue(kitap_no);
    sorgu->exec();

    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tableView_kitaplar_onceden->setModel(model1);

    sorgu->prepare("select * from odunc_teslim_edilen where kitap_no=?");
    sorgu->addBindValue(kitap_no);
    sorgu->exec();

    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tableView_kitap_odunc->setModel(model1);

}


void kitap_islemleri::on_btn_kaydet_clicked()
{
    QLineEdit* lineEdit = findChild<QLineEdit*>("lineEdit_ad");
    QString text = lineEdit->text();

    QLineEdit* lineEdit_2 = findChild<QLineEdit*>("lineEdit_stok");
    QString text_2 = lineEdit_2->text();
    if (text.isEmpty() || text_2.isEmpty()) {
        QMessageBox::critical(this,"Hata!","Gerekli alanları doldurunuz!","Ok");
        return;
    }
    else
    {
    sorgu->prepare("insert into kitap(kitap_ad,kitap_sayisi) values(?,?)");
    sorgu->addBindValue(ui->lineEdit_ad->text());
    sorgu->addBindValue(ui->lineEdit_stok->text());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap islemleri tablosuna ekleme yapılırken bir hata ile karşılaşıldı","Ok");
        return;
    }
    listele();
    }
}

void kitap_islemleri::on_btn_guncelle_clicked()
{
    sorgu->prepare("update kitap set kitap_ad=?,kitap_sayisi=? where kitap_no=?");
    sorgu->addBindValue(ui->lineEdit_ad->text());
    sorgu->addBindValue(ui->lineEdit_stok->text());
    sorgu->addBindValue(ui->lineEdit_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap islemleri tablosunda guncelleme yapılırken bir hata ile karşılaşıldı","Ok");
        return;
    }
    listele();
}


void kitap_islemleri::on_btn_sil_clicked()
{
    sorgu->prepare("select * from odunc_alinan where kitap_no=?");
    sorgu->addBindValue(ui->lineEdit_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap islemleri tablosundan veri alınırken bir hata ile karşılaşıldı","Ok");
        return;
    }
    int syc=0;
    while(sorgu->next())
    {
        syc++;
    }
    if(syc>0)
    {
        QMessageBox::critical(this,"Hata!","Bu kitap silinemez.Bu kitap bir üyeye ödünç verilmiştir.","Ok");
        return;
    }
    else
    {
        sorgu->prepare("delete from kitap where kitap_no=?");
        sorgu->addBindValue(ui->lineEdit_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap islemleri tablosunda silme yapılırken bir hata ile karşılaşıldı","Ok");
        return;
    }
    }
    listele();
}




//İREM SILA SARIKAYA 21100011030

#include "odunc_alma.h"
#include "ui_odunc_alma.h"

odunc_alma::odunc_alma(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::odunc_alma)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    listele();
}

odunc_alma::~odunc_alma()
{
    delete ui;
}

void odunc_alma::listele()
{
    sorgu->prepare("select * from odunc_alinan");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ödünç alma tablosu çekilirken bir hata ile karşılaşıldı","Ok");
        return;
    }

    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_odunc_alnan->setModel(model);

    sorgu->prepare("select * from uye");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Üye tablosu çekilirken bir hata ile karşılaşıldı","Ok");
        return;
    }

    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tableView_tum_uyeler->setModel(model1);


    sorgu->prepare("select * from kitap");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap işlemleri tablosu çekilirken bir hata ile karşılaşıldı","Ok");
        return;
    }

    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_tum_kitaplar->setModel(model);


}

void odunc_alma::on_tableView_tum_kitaplar_clicked(const QModelIndex &index)
{
    ui->lineEdit_kitap_no->setText(model->index(index.row(),0).data().toString());
    stok = model->index(index.row(),2).data().toInt();

}

void odunc_alma::on_tableView_tum_uyeler_clicked(const QModelIndex &index)
{
    ui->lineEdit_uye_no->setText(model1->index(index.row(),0).data().toString());
    uye=model1->index(index.row(),0).data().toString();
}



void odunc_alma::on_btn_odunc_al_clicked()
{

    sorgu->prepare("select * from odunc_alinan where uye_no=? and kitap_no=?");
    sorgu->addBindValue(ui->lineEdit_uye_no->text().toInt());
    sorgu->addBindValue(ui->lineEdit_kitap_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Uye tablosundan veri çekerken bir hata ile karşılaşıldı","Ok");
        return;
    }
    int syc=0;
    while(sorgu->next())
    {
        syc++;
    }

    sorgu->prepare("select * from odunc_alinan where kitap_no=?");
    sorgu->addBindValue(ui->lineEdit_kitap_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Uye tablosundan veri çekerken bir hata ile karşılaşıldı","Ok");
        return;
    }
    int syc1=0;
    while(sorgu->next())
    {
        syc1++;
    }

    if(syc>0)
    {
        QMessageBox::critical(this,"Hata!","Bu üye bu kitabın bir tanesini şu an ödünç almış tekrar ödünç verilemez.","Ok");
        return;
    }
    else if(syc1>=stok)
    {
        QMessageBox::critical(this,"Hata!","Bu kitap ödünç verilemez yeterli stok bulunmamaktadır.","Ok");
        return;
    }
    else
    {
        sorgu->prepare("insert into odunc_alinan(uye_no,kitap_no,odunc_alma_tarihi) values(?,?,?)");
        sorgu->addBindValue(ui->lineEdit_uye_no->text());
        sorgu->addBindValue(ui->lineEdit_kitap_no->text());
        sorgu->addBindValue(ui->dateEdit->text());
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"Hata!","Kitap ödünç alınırken bir hata ile karşılaşıldı","Ok");
            return;
        }
    }

    listele();
}




//İREM SILA SARIKAYA 21100011030

#include "odunc_teslim_etme.h"
#include "ui_odunc_teslim_etme.h"

odunc_teslim_etme::odunc_teslim_etme(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::odunc_teslim_etme)
{
    ui->setupUi(this);
    sorgu = new QSqlQuery(db);
    listele();
}

odunc_teslim_etme::~odunc_teslim_etme()
{
    delete ui;
}

void odunc_teslim_etme::listele()
{
    sorgu->prepare("select * from odunc_alinan");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ödünç alma işlemleri tablosu çekilirken bir hata ile karşılaşıldı","Ok");
        return;
    }

    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_odunc_alinan->setModel(model);

    sorgu->prepare("select * from odunc_teslim_edilen");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ödünç teslim etme işlemleri tablosu çekilirken bir hata ile karşılaşıldı","Ok");
        return;
    }

    model2 = new QSqlQueryModel();
    model2->setQuery(*sorgu);
    ui->tableView_teslim_edilen->setModel(model2);
}

void odunc_teslim_etme::on_tableView_odunc_alinan_clicked(const QModelIndex &index)
{
    ui->lineEdit_uye_no_3->setText(model->index(index.row(),0).data().toString());
    ui->lineEdit_kitap_no_3->setText(model->index(index.row(),1).data().toString());
    odunc = model->index(index.row(),2).data().toString();
}


void odunc_teslim_etme::on_btn_odunc_ver_clicked()
{
    sorgu->prepare("insert into odunc_teslim_edilen(uye_no,kitap_no,alma_tarihi,verme_tarihi,borc) values(?,?,?,?,?)");
    sorgu->addBindValue(ui->lineEdit_uye_no_3->text());
    sorgu->addBindValue(ui->lineEdit_kitap_no_3->text());
    sorgu->addBindValue(odunc);
    sorgu->addBindValue(ui->dateEdit_2->text());

    QStringList dateParts = odunc.split('.');

    if (dateParts.size() == 3) {
        day = dateParts[0].toInt();
        month = dateParts[1].toInt();
        year = dateParts[2].toInt();
    }
    borc = (ui->dateEdit_2->date().day() - day) + (ui->dateEdit_2->date().month() - month)*30 + (ui->dateEdit_2->date().year() - year)*360;


    if(borc<=15)
    {
        borc=0;
    }
    else
    {
        borc = borc-15;
    }
    sorgu->addBindValue(borc);
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ödünç teslim etme tablosuna ekleme yapılırken bir hata ile karşılaşıldı","Ok");
        return;
    }

    sorgu->prepare("delete from odunc_alinan where uye_no=? and kitap_no=?");
    sorgu->addBindValue(ui->lineEdit_uye_no_3->text().toInt());
    sorgu->addBindValue(ui->lineEdit_kitap_no_3->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","silme yapılırken bir hata ile karşılaşıldı","Ok");
        return;
    }

    listele();
}




//İREM SILA SARIKAYA 21100011030

#include "uye_islemleri.h"
#include "ui_uye_islemleri.h"

uye_islemleri::uye_islemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uye_islemleri)
{
    ui->setupUi(this);
    sorgu = new QSqlQuery(db);
    listele();
}

uye_islemleri::~uye_islemleri()
{
    delete ui;
}

void uye_islemleri::listele()
{
    sorgu->prepare("select * from uye");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","uye tablosu çekilirken bir hata ile karşılaşıldı","Ok");
        return;
    }

    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_tum_uyeler->setModel(model);
}

void uye_islemleri::on_tableView_tum_uyeler_clicked(const QModelIndex &index)
{
    ui->lineEdit_no->setText(model->index(index.row(),0).data().toString());
    ui->lineEdit_ad->setText(model->index(index.row(),1).data().toString());
    ui->lineEdit_soyad->setText(model->index(index.row(),2).data().toString());
}


void uye_islemleri::on_btn_yeni_kayit_clicked()
{
    QLineEdit* lineEdit = findChild<QLineEdit*>("lineEdit_ad");
    QString text = lineEdit->text();

    QLineEdit* lineEdit_2 = findChild<QLineEdit*>("lineEdit_soyad");
    QString text_2 = lineEdit_2->text();
    if (text.isEmpty() || text_2.isEmpty()) {
        QMessageBox::critical(this,"Hata!","Gerekli alanları doldurunuz!","Ok");
        return;
    }
    else
    {
    sorgu->prepare("insert into uye(uye_ad,uye_soyad) values(?,?)");
    sorgu->addBindValue(ui->lineEdit_ad->text());
    sorgu->addBindValue(ui->lineEdit_soyad->text());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Uye tablosuna ekleme yapılırken bir hata ile karşılaşıldı","Ok");
        return;
    }
    listele();

    }

}


void uye_islemleri::on_btn_guncelle_clicked()
{
    sorgu->prepare("update uye set uye_ad=?,uye_soyad=? where uye_no=?");
    sorgu->addBindValue(ui->lineEdit_ad->text());
    sorgu->addBindValue(ui->lineEdit_soyad->text());
    sorgu->addBindValue(ui->lineEdit_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Uye tablosunda guncelleme yapılırken bir hata ile karşılaşıldı","Ok");
        return;
    }
    listele();

}




void uye_islemleri::on_btn_sil_clicked()
{
    sorgu->prepare("select * from odunc_alinan where uye_no=?");
    sorgu->addBindValue(ui->lineEdit_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Uye tablosundan veri çekerken bir hata ile karşılaşıldı","Ok");
        return;
    }
    int syc=0;
    while(sorgu->next())
    {
        syc++;
    }
    if(syc>0)
    {
        QMessageBox::critical(this,"Hata!","Bu üye silinemez. Üyenin hala teslim etmediği kitaplar var.","Ok");
        return;
    }
    else
    {
        sorgu->prepare("delete from uye where uye_no=?");
        sorgu->addBindValue(ui->lineEdit_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Uye tablosunda silme yapılırken bir hata ile karşılaşıldı","Ok");
        return;
    }
    }
    listele();
}

