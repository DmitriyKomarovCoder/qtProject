#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_products.h"
#include "sales.h"



#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    windowUser = new User();
    connect(windowUser, &User::mainWindow, this, &MainWindow::show);

    windowAdmin = new Admin();
    connect(windowAdmin, &Admin::mainWindow, this, &MainWindow::show);


    //connect(ui->comboBox,SIGNAL(activated(int)),this, SLOT(chooseRole(int)));
    connect(ui->btnConnect,SIGNAL(clicked(bool)),this, SLOT(dbconnect()));


//    connect(ui->btnSelectAll,SIGNAL(clicked(bool)),this, SLOT(selectAll()));
//    connect(ui->btnProdcuts,SIGNAL(clicked(bool)),this, SLOT(openProducts()));
//    connect(ui->btnSales,SIGNAL(clicked(bool)),this, SLOT(openSales()));
//    connect(ui->btnSellers,SIGNAL(clicked(bool)),this, SLOT(openSellers()));
//    connect(ui->btnCustomers,SIGNAL(clicked(bool)),this, SLOT(openCustomers()));
//    connect(ui->btnSuppliers,SIGNAL(clicked(bool)),this, SLOT(openSuppliers()));
//    connect(ui->btnWarehouses,SIGNAL(clicked(bool)),this, SLOT(openWarehouses()));

}

MainWindow::~MainWindow()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
    //delete windowUser;
    //delete windowAdmin;
}


void MainWindow::dbconnect()
{
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Введите пароль!");
        return;
    }
    if (ui->comboBox->currentIndex() == 0) {
        if (ui->lineEdit->text().toInt() == 111) {
            windowAdmin->show();
            this->close();
            //открыть окно админа
        } else {
            QMessageBox::critical(this,"Error","Введен неправильный пароль админа!");
            return;
        }
    } else if (ui->comboBox->currentIndex() == 1) {
        if (ui->lineEdit->text().toInt() == 222) {
            windowUser->show();
            this->close();
            //открыть окно покупателя
        } else {
            QMessageBox::critical(this,"Error","Введен неправильный пароль пользователя!");
            return;
        }
    }

    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teResult->append("SQL drivers:");
        ui->teResult->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД

        dbconn=QSqlDatabase::addDatabase("QPSQL", "main_connection");

        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль
        //пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("Retail chains");
        dbconn.setHostName("localhost");
        dbconn.setUserName("kosmatoff");
        dbconn.setPassword("2003");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teResult->append("Connect is open...");
        else
        {
            ui->teResult->append("Error of connect:");
            ui->teResult->append(dbconn.lastError().text());
        }
    }
    else
    // Если соединение уже открыто, то сообщить об этом
        ui->teResult->append("Connect is already open...");
}


