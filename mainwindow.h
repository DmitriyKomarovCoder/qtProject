#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlDatabase>
#include <QMainWindow>

#include <dialog_products.h>
#include "sales.h"
#include "seller.h"
#include "supplier.h"
#include "customer.h"
#include "warehouse.h"
#include "admin.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    User* windowUser;
    Admin* windowAdmin;
    //Dialog_Products dialogProducts;
    //Sales sales;
    //Seller seller;
    //Customer customer;
    //Supplier supplier;
    //Warehouse warehouse;
    QSqlDatabase dbconn;

public slots:
    void dbconnect();
    //void selectAll();
    //void openProducts();
    //void openSales();
    //void openSellers();
    //void openCustomers();
    //void openSuppliers();
    //void openWarehouses();
};
#endif // MAINWINDOW_H
