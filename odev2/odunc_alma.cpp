
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



