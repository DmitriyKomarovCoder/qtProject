#include "user.h"
#include "ui_user.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>


User::User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);

    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));

    connect(ui->btnProducts,SIGNAL(clicked(bool)),this, SLOT(selectProducts()));

    ui->twOrg->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    //ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("id"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("name"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("price"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void User::showBack() {
    this->close();
    emit mainWindow();

}

void User::showExit() {
    this->close();
}

void User::dbconnect()
{
    if(!dbconn.isOpen())
    {
        dbconn=QSqlDatabase::database("main_connection");

    }
}

void User::selectProducts() {
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
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from Products";
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
        //ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("id").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("name").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("price").toString()));
        i++;
    }
}

User::~User()
{
    delete ui;
}
