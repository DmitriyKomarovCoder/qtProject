#ifndef SALES_H
#define SALES_H

#include <QDialog>
#include <QSqlDatabase>

//#include "seller.h"

namespace Ui {
class Sales;
}

class Sales : public QDialog
{
    Q_OBJECT

public:
    explicit Sales(QWidget *parent = nullptr);
    ~Sales();

private:
    Ui::Sales *ui;
    QSqlDatabase dbconn;
    //Seller* windowSeller;

signals:
    void formsWindow();
    void sellersWindow();

public slots:
    void selectSales();
    void add();
    void del();
    void edit();
    void dbconnect();
    void showBack();
    void showExit();
};


#endif // SALES_H
