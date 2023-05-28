#include "warehouse.h"
#include "ui_warehouse.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>


void Warehouse::showBack() {
    this->close();
    emit formsWindow();

}

void Warehouse::showExit() {
    this->close();
}

Warehouse::Warehouse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Warehouse)
{
    ui->setupUi(this);
    connect(ui->btnSales,SIGNAL(clicked(bool)),this, SLOT(selectWarehouses()));
    connect(ui->btnAdd_2,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->btnDel_2,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->btnEdit_2,SIGNAL(clicked(bool)),this, SLOT(edit()));
    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));

    connect(ui->btnCountSupplier,SIGNAL(clicked(bool)),this, SLOT(selectCount()));

    ui->twOrg->setColumnCount(4);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("id"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("email"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("address"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Supplier"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Warehouse::~Warehouse()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}

void Warehouse::dbconnect()
{
    if(!dbconn.isOpen())
    {
        dbconn=QSqlDatabase::database("main_connection");

    }
}

void Warehouse::selectCount() {
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


    QString sqlstr = "SELECT * FROM Warehouse WHERE supplier_id = ?";
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->lineEditCount->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поле перед тем как делать запрос!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->lineEditCount->text().toInt());

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
    ui->teResult->append( QString("Read %1 rows").arg(query.size()));

    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("id").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("email").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("address").toString()));
        ui->twOrg->setItem(i,3,new QTableWidgetItem(query.value("supplier_id").toString()));
        i++;
    }
}

void Warehouse::selectWarehouses()
{
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
    QString sqlstr = "select * from Warehouse";
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
    ui->teResult->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twOrg->setItem(i,0,new QTableWidgetItem(query.value("id").toString()));
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("email").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("address").toString()));
        ui->twOrg->setItem(i,3,new QTableWidgetItem(query.value("supplier_id").toString()));
        i++;
    }
}

void Warehouse::add()
{
    // Подключиться к БД
    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into Warehouse (id, email, address, supplier_id) values(?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->leAbbr->text().isEmpty() or ui->teTitle->text().isEmpty() or ui->leCity->text().isEmpty() or ui->lineEdit->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поля перед тем как добавлять строчку!");
        return;
    }
    if (ui->leAbbr->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле id не может быть меньше 1!");
        return;
    }
    if (ui->lineEdit->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле id_supplier не может быть меньше 1!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leAbbr->text().toInt());
    query.bindValue(1,ui->teTitle->text());
    query.bindValue(2,ui->leCity->text());
    query.bindValue(3,ui->lineEdit->text().toInt());
    // Выполнить запрос
    if( !query.exec() )
    {
        ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Если запрос выполнен, то вывести сообщение одобавлении строки
    ui->teResult->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
    // и обновить записи в компоненте таблицы
    selectWarehouses();
}

void Warehouse::del()
{
    // Подключение к БД
    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    // Получить номер выбранной строки в компоненте таблицы
    int currow = ui->twOrg->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    // Спросить у пользователя подтверждение удаления записи
    // Используется статический метод QMessageBox::question
    // для задания вопроса, который возвращает код нажатой кнопки

    if( QMessageBox::question(this,"Delete","Delete row?",QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    QString sqlstr = "delete from Warehouse where id = '" + ui->twOrg->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        //ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    ui->teResult->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    selectWarehouses();
}

void Warehouse::edit()
{
    //Подключиться к БД
    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    int currow = ui->twOrg->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }

    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "UPDATE Warehouse SET id = ?, email = ?, address= ?, supplier_id = ? WHERE id ='" + ui->twOrg->item(currow,0)->text() + "'";;
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->leAbbr->text().isEmpty() or ui->teTitle->text().isEmpty() or ui->leCity->text().isEmpty() or ui->lineEdit->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поля перед тем как редактировать строчку!");
        return;
    }
    if (ui->leAbbr->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле id не может быть меньше 1!");
        return;
    }
    if (ui->lineEdit->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле id_supplier не может быть меньше 1!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leAbbr->text().toInt());
    query.bindValue(1,ui->teTitle->text());
    query.bindValue(2,ui->leCity->text());
    query.bindValue(3,ui->lineEdit->text().toInt());
    // Выполнить запрос
    if( !query.exec() )
    {
       //ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error","Нельзя изменить id, тк на данную строку ссылается строка из другой таблицы!");
        return;
    }
    // Если запрос выполнен, то вывести сообщение одобавлении строки
    ui->teResult->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
    // и обновить записи в компоненте таблицы
    selectWarehouses();

}


