
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

