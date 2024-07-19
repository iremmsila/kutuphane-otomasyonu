#ifndef ODUNC_ALMA_H
#define ODUNC_ALMA_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlTableModel>

namespace Ui {
class odunc_alma;
}

class odunc_alma : public QDialog
{
    Q_OBJECT

public:
    explicit odunc_alma(QSqlDatabase,QWidget *parent = nullptr);
    ~odunc_alma();
    void listele();

private slots:
    void on_tableView_tum_uyeler_clicked(const QModelIndex &index);

    void on_tableView_tum_kitaplar_clicked(const QModelIndex &index);

    void on_btn_odunc_al_clicked();

private:
    Ui::odunc_alma *ui;
    QSqlQuery *sorgu;
    QSqlQuery *sorgu1;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QString uye;
    int stok;
};

#endif // ODUNC_ALMA_H
