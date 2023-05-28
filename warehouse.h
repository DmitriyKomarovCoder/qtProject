#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <QDialog>
#include <QSqlDatabase>

//#include "supplier.h"

namespace Ui {
class Warehouse;
}

class Warehouse : public QDialog
{
    Q_OBJECT

public:
    explicit Warehouse(QWidget *parent = nullptr);
    ~Warehouse();

private:
    Ui::Warehouse *ui;
    QSqlDatabase dbconn;
    //Supplier* a;

signals:
    void formsWindow();

public slots:
    void selectWarehouses();
    void add();
    void del();
    void edit();
    void dbconnect();
    void showBack();
    void showExit();
    void selectCount();
};

#endif // WAREHOUSE_H
