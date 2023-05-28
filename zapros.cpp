#include "zapros.h"
#include "ui_zapros.h"


#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>


void Zapros::showBack() {
    this->close();
    emit adminWindow();

}

void Zapros::showExit() {
    this->close();
}


Zapros::Zapros(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Zapros)
{
    ui->setupUi(this);
    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));

    connect(ui->btnCustomerInfo,SIGNAL(clicked(bool)),this, SLOT(selectCustomersInfo()));
    connect(ui->btnSalesSellers,SIGNAL(clicked(bool)),this, SLOT(selectSalesSallers()));
    connect(ui->btnSupplierFio,SIGNAL(clicked(bool)),this, SLOT(selectFioSuppliers()));
    connect(ui->btnSort,SIGNAL(clicked(bool)),this, SLOT(selectSort()));
    connect(ui->btnSum,SIGNAL(clicked(bool)),this, SLOT(selectSum()));
    connect(ui->btnMore,SIGNAL(clicked(bool)),this, SLOT(selectCount()));

}

Zapros::~Zapros()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}

void Zapros::dbconnect()
{
    if(!dbconn.isOpen())
    {
        dbconn=QSqlDatabase::database("main_connection");

    }
}

void Zapros::selectCount() {
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


    QString sqlstr = "SELECT * FROM Products WHERE price > ?";
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->lineEdit_2->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поле перед тем как делать запрос!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->lineEdit_2->text().toFloat());

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


void Zapros::selectSum()
{
    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и
    ui->twOrg->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("id Товара"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Название товара"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Сумма всех продаж"));


    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    // Создать строку запроса на выборку данных
    QString sqlstr = "SELECT p.id AS product_id, p.name AS product_name, SUM(s.total_price) AS total_sales FROM Products p JOIN Sales s ON p.id = s.product_id GROUP BY p.id, p.name";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
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
        ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("product_id").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("product_name").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("total_sales").toString()));
        i++;
    }
}



void Zapros::selectSort()
{
    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и
    ui->twOrg->setColumnCount(8);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Номер продажи"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Дата"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Название продукта"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Количество"));
    ui->twOrg->setHorizontalHeaderItem(4,new QTableWidgetItem("Стоимость"));
    ui->twOrg->setHorizontalHeaderItem(5,new QTableWidgetItem("Имя поставщика"));
    ui->twOrg->setHorizontalHeaderItem(6,new QTableWidgetItem("Email"));
    ui->twOrg->setHorizontalHeaderItem(7,new QTableWidgetItem("Адрес поставщика"));


    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    // Создать строку запроса на выборку данных
    QString sqlstr = "SELECT s.id AS sale_id, s.date, p.name AS product_name, s.quantity, s.total_price, sup.full_name AS supplier_name, sup.email AS supplier_email, sup.address AS supplier_address FROM Sales s JOIN Products p ON s.product_id = p.id JOIN Supplier sup ON p.id = sup.id ORDER BY s.total_price DESC";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
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
        ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("sale_id").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("date").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("product_name").toString()));
        ui->twOrg->setItem(i,3,new QTableWidgetItem(query.value("quantity").toString()));
        ui->twOrg->setItem(i,4,new QTableWidgetItem(query.value("total_price").toString()));
        ui->twOrg->setItem(i,5,new QTableWidgetItem(query.value("supplier_name").toString()));
        ui->twOrg->setItem(i,6,new QTableWidgetItem(query.value("supplier_email").toString()));
        ui->twOrg->setItem(i,7,new QTableWidgetItem(query.value("supplier_address").toString()));
        i++;
    }
}

void Zapros::selectFioSuppliers()
{
    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и
    ui->twOrg->setColumnCount(4);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("id"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Почта"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("ФИО"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Адрес"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

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


    QString sqlstr = "SELECT * FROM Supplier WHERE full_name LIKE '%" + ui->lineEdit->text() + "%'";
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поле перед тем как делать запрос!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->lineEdit->text());

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
        ui->twOrg->setItem(i,3,new QTableWidgetItem(query.value("address").toString()));
        i++;
    }
}



void Zapros::selectSalesSallers()
{
    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и
    ui->twOrg->setColumnCount(5);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Дата"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Название продукта"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Количество"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Стоимость"));
    ui->twOrg->setHorizontalHeaderItem(4,new QTableWidgetItem("Имя покупателя"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    // Создать строку запроса на выборку данных
    QString sqlstr = "SELECT s.date, p.name AS product_name, s.quantity, s.total_price, sup.full_name AS supplier_name FROM Sales AS s JOIN Products AS p ON s.product_id = p.id JOIN Supplier AS sup ON p.id = sup.id";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
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
        ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("date").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("product_name").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("quantity").toString()));
        ui->twOrg->setItem(i,3,new QTableWidgetItem(query.value("total_price").toString()));
        ui->twOrg->setItem(i,4,new QTableWidgetItem(query.value("supplier_name").toString()));
        i++;
    }
}




void Zapros::selectCustomersInfo()
{
    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и
    ui->twOrg->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("id"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("email"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Full name"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from Customer";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
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

