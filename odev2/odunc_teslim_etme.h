#ifndef ODUNC_TESLIM_ETME_H
#define ODUNC_TESLIM_ETME_H

#include "qdatetime.h"
#include "qdatetimeedit.h"
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlTableModel>

namespace Ui {
class odunc_teslim_etme;
}

class odunc_teslim_etme : public QDialog
{
    Q_OBJECT

public:
    explicit odunc_teslim_etme(QSqlDatabase, QWidget *parent = nullptr);
    ~odunc_teslim_etme();
    void listele();

private slots:
    void on_tableView_odunc_alinan_clicked(const QModelIndex &index);

    void on_btn_odunc_ver_clicked();

private:
    Ui::odunc_teslim_etme *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
    QSqlQueryModel *model2;
    QString odunc;
    int borc;
    int day;
    int month;
    int year;
};

#endif // ODUNC_TESLIM_ETME_H
