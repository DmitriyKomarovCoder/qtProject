#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <QDialog>
#include <QSqlDatabase>


namespace Ui {
class Supplier;
}

class Supplier : public QDialog
{
    Q_OBJECT

public:
    explicit Supplier(QWidget *parent = nullptr);
    ~Supplier();

private:
    Ui::Supplier *ui;
    QSqlDatabase dbconn;
    //Warehouse* a;

signals:
    void formsWindow();

public slots:
    void selectSuppliers();
    void add();
    void del();
    void edit();
    void dbconnect();
    void showBack();
    void showExit();
};

#endif // SUPPLIER_H
