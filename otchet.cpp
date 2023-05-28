#include "otchet.h"
#include "ui_otchet.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>

void Otchet::showBack() {
    this->close();
    emit adminWindow();

}

void Otchet::showExit() {
    this->close();
}


Otchet::Otchet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Otchet)
{
    ui->setupUi(this);

    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));

    connect(ui->btnSum10,SIGNAL(clicked(bool)),this, SLOT(selectSum10()));
    connect(ui->btnSallersInfo,SIGNAL(clicked(bool)),this, SLOT(selectSellers()));
    connect(ui->btnCustomerInfo,SIGNAL(clicked(bool)),this, SLOT(selectCustomers()));
}

Otchet::~Otchet()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}

void Otchet::dbconnect()
{
    if(!dbconn.isOpen())
    {
        dbconn=QSqlDatabase::database("main_connection");

    }
}

void Otchet::selectSellers() {
    ui->twOrg->clearContents();

    ui->twOrg->setColumnCount(2);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Имя продавца"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Сумма продаж каждого продавца"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и

    // выйти из функции
    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    // Количество столбцов

    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);


    QString sqlstr = "SELECT s.full_name AS seller_name, SUM(sa.total_price) AS total_revenue FROM Sales sa JOIN Seller s ON sa.seller_id = s.id GROUP BY s.id ORDER BY total_revenue DESC";
    // Подготовить запрос
    query.prepare(sqlstr);

    // Выполнить запрос
    if(!query.exec()){
        //qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if( query.isActive())
        ui->twOrg->setRowCount( query.size());
    else
        ui->twOrg->setRowCount(0);
    //ui->teResult->append( QString("Read %1 rows").arg(query.size()));

    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("seller_name").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("total_revenue").toString()));
        i++;
    }

}

void Otchet::selectCustomers() {
    ui->twOrg->clearContents();

    ui->twOrg->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Номер поставщика"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Email"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("ФИО Поставщика"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и

    // выйти из функции
    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    // Количество столбцов

    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);


    QString sqlstr = "SELECT * FROM Customer";
    // Подготовить запрос
    query.prepare(sqlstr);

    // Выполнить запрос
    if(!query.exec()){
        //qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if( query.isActive())
        ui->twOrg->setRowCount( query.size());
    else
        ui->twOrg->setRowCount(0);
    //ui->teResult->append( QString("Read %1 rows").arg(query.size()));

    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("id").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("email").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("full_name").toString()));
        i++;
    }

}


void Otchet::selectSum10() {
    ui->twOrg->clearContents();

    ui->twOrg->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Номер товара"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Название товара"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Стоймость"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и

    // выйти из функции
    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    // Количество столбцов

    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);


    QString sqlstr = "SELECT * FROM Products WHERE price > 10";
    // Подготовить запрос
    query.prepare(sqlstr);

    // Выполнить запрос
    if(!query.exec()){
        //qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if( query.isActive())
        ui->twOrg->setRowCount( query.size());
    else
        ui->twOrg->setRowCount(0);
    //ui->teResult->append( QString("Read %1 rows").arg(query.size()));

    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("id").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("name").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("price").toString()));
        i++;
    }

}

