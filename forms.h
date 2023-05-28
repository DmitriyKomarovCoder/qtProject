#ifndef FORMS_H
#define FORMS_H

#include <QDialog>

#include "customer.h"
#include "dialog_products.h"
#include "sales.h"
#include "supplier.h"
#include "warehouse.h"
#include "seller.h"

namespace Ui {
class Forms;
}

class Forms : public QDialog
{
    Q_OBJECT

public:
    explicit Forms(QWidget *parent = nullptr);
    ~Forms();

signals:
    void adminWindow();

private:
    Ui::Forms *ui;
    Customer* customerWindow;
    Dialog_Products* productsWindow;
    Sales* salesWindow;
    Supplier* supplierWindow;
    Warehouse* warehouseWindow;
    Seller* sellerWindow;

public slots:
    void showExit();
    void showBack();
    void openProducts();
    void openSales();
    void openSellers();
    void openCustomers();
    void openSuppliers();
    void openWarehouses();

};

#endif // FORMS_H
