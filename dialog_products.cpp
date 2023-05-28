#include "dialog_products.h"
#include "ui_dialog_products.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>


void Dialog_Products::showBack() {
    this->close();
    emit formsWindow();

}

void Dialog_Products::showExit() {
    this->close();
}

Dialog_Products::Dialog_Products(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Products)
{
    ui->setupUi(this);
    connect(ui->btnProduct,SIGNAL(clicked(bool)),this, SLOT(selectProduct()));
    connect(ui->btnAdd_2,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->btnDel_2,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->btnEdit_2,SIGNAL(clicked(bool)),this, SLOT(edit()));
    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));

    connect(ui->btnCount,SIGNAL(clicked(bool)),this, SLOT(selectCount()));

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

void Dialog_Products::selectCount() {
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
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поле перед тем как делать запрос!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->lineEdit->text().toFloat());

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
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("name").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("price").toString()));
        i++;
    }

}

Dialog_Products::~Dialog_Products()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}

void Dialog_Products::dbconnect()
{
    if(!dbconn.isOpen())
    {
        dbconn=QSqlDatabase::database("main_connection");

    }
}

void Dialog_Products::selectProduct()
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
    ui->teResult->append( QString("Read %1 rows").arg(query.size()));
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

void Dialog_Products::add()
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
    QString sqlstr = "insert into Products (id, name, price) values(?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->leAbbr->text().isEmpty() or ui->teTitle->toPlainText().isEmpty() or ui->leCity->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поля перед тем как добавлять строчку!");
        return;
    }
    if (ui->leAbbr->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле id не может быть меньше 1!");
        return;
    }
    if (ui->leCity->text().toFloat() < 0) {
        QMessageBox::critical(this,"Error","Поле price не может быть меньше 0!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leAbbr->text());
    query.bindValue(1,ui->teTitle->toPlainText());
    query.bindValue(2,ui->leCity->text().toFloat());
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
    selectProduct();
}

void Dialog_Products::del()
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
    QString sqlstr = "delete from Products where id = '" + ui->twOrg->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    ui->teResult->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    selectProduct();
}

void Dialog_Products::edit()
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
    QString sqlstr = "UPDATE Products SET id = ?, name = ?, price = ? WHERE id ='" + ui->twOrg->item(currow,0)->text() + "'";
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->leAbbr->text().isEmpty() or ui->teTitle->toPlainText().isEmpty() or ui->leCity->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поля перед тем как редактировать строчку!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leAbbr->text());
    query.bindValue(1,ui->teTitle->toPlainText());
    query.bindValue(2,ui->leCity->text().toFloat());
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
    selectProduct();

}

