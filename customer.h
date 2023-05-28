#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QDialog>
#include <QSqlDatabase>

//#include "seller.h"

namespace Ui {
class Customer;
}

class Customer : public QDialog
{
    Q_OBJECT

public:
    explicit Customer(QWidget *parent = nullptr);
    ~Customer();
    //Seller* windowSellers;

private:
    Ui::Customer *ui;
    QSqlDatabase dbconn;
    //Forms* a;
    //Seller* windowSellers;

signals:
    void formsWindow();
    void sellersWindow();

public slots:
    void selectCustomers();
    void add();
    void del();
    void edit();
    void dbconnect();
    void showBack();
    void showExit();
    void showSellers();
};

#endif // CUSTOMER_H
