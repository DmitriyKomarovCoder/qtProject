#include "admin.h"
#include "ui_admin.h"

Admin::Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
    windowForms = new Forms();
    connect(windowForms, &Forms::adminWindow, this, &Admin::show);

    windowOtchet = new Otchet();
    connect(windowOtchet, &Otchet::adminWindow, this, &Admin::show);

    windowZapros = new Zapros();
    connect(windowZapros, &Zapros::adminWindow, this, &Admin::show);

    connect(ui->btnForm,SIGNAL(clicked(bool)),this, SLOT(showForm()));
    connect(ui->btnOtchet,SIGNAL(clicked(bool)),this, SLOT(showOtchet()));
    connect(ui->btnZapros,SIGNAL(clicked(bool)),this, SLOT(showZapros()));
    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));
}

Admin::~Admin()
{
    delete ui;
    delete windowForms;
    delete windowOtchet;
    delete windowZapros;
}

void Admin::showForm() {
    windowForms->show();
    this->close();

}

void Admin::showOtchet() {
    windowOtchet->show();
    this->close();

}

void Admin::showZapros() {
    windowZapros->show();
    this->close();

}

void Admin::showBack() {
    this->close();
    emit mainWindow();

}

void Admin::showExit() {
    this->close();
}

