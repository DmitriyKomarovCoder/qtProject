#include "forms.h"
#include "ui_forms.h"

Forms::Forms(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Forms)
{
    ui->setupUi(this);

    customerWindow = new Customer();
    connect(customerWindow, &Customer::formsWindow, this, &Forms::show);

    productsWindow = new Dialog_Products();
    connect(productsWindow, &Dialog_Products::formsWindow, this, &Forms::show);

    salesWindow = new Sales();
    connect(salesWindow, &Sales::formsWindow, this, &Forms::show);

    supplierWindow = new Supplier();
    connect(supplierWindow, &Supplier::formsWindow, this, &Forms::show);

    warehouseWindow = new Warehouse();
    connect(warehouseWindow, &Warehouse::formsWindow, this, &Forms::show);

    sellerWindow = new Seller();
    connect(sellerWindow , &Seller::formsWindow, this, &Forms::show);

    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));

    connect(ui->btnProdcuts,SIGNAL(clicked(bool)),this, SLOT(openProducts()));
    connect(ui->btnSales,SIGNAL(clicked(bool)),this, SLOT(openSales()));
    connect(ui->btnSellers,SIGNAL(clicked(bool)),this, SLOT(openSellers()));
    connect(ui->btnCustomers,SIGNAL(clicked(bool)),this, SLOT(openCustomers()));
    connect(ui->btnSuppliers,SIGNAL(clicked(bool)),this, SLOT(openSuppliers()));
    connect(ui->btnWarehouses,SIGNAL(clicked(bool)),this, SLOT(openWarehouses()));

}

Forms::~Forms()
{
    delete ui;
    delete customerWindow;
    delete productsWindow;
    delete salesWindow;
    delete supplierWindow;
    delete warehouseWindow;
    delete sellerWindow;

}


void Forms::showBack() {
    this->close();
    emit adminWindow();

}

void Forms::showExit() {
    this->close();
}

void Forms::openProducts()
{
    productsWindow->show();
    this->close();
}

void Forms::openSales()
{
    salesWindow->show();
    this->close();
}

void Forms::openSellers()
{
    sellerWindow->setCombo();
    sellerWindow->show();
    this->close();
}

void Forms::openCustomers()
{
    customerWindow->show();
    this->close();
}

void Forms::openSuppliers()
{
    supplierWindow->show();
    this->close();
}

void Forms::openWarehouses()
{
    warehouseWindow->show();
    this->close();
}

