
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

