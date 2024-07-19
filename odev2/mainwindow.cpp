
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

