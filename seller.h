#ifndef SELLER_H
#define SELLER_H

#include <QDialog>
#include <QSqlDatabase>

#include "customer.h"

namespace Ui {
class Seller;
}

class Seller : public QDialog
{
    Q_OBJECT

public:
    explicit Seller(QWidget *parent = nullptr);
    ~Seller();

private:
    Ui::Seller *ui;
    QSqlDatabase dbconn;
    Customer* windowCustomers;
    int selectedShopIndex;
signals:
    void formsWindow();
    void customersWindow();

public slots:
    void handleSelection(int index);
    void selectSellers();
    void add();
    void del();
    void edit();
    void dbconnect();
    void showBack();
    void showExit();
    void showCustomers();
    void getId();
    void setCombo();
    void clearCombo();
    void search();
};

#endif // SELLER_H
