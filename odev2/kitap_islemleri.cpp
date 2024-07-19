
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


