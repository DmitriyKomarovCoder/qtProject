#include "sales.h"
#include "ui_sales.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>


void Sales::showBack() {
    this->close();
    emit formsWindow();

}

void Sales::showExit() {
    this->close();
}

Sales::Sales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sales)
{
    ui->setupUi(this);
    connect(ui->btnSales,SIGNAL(clicked(bool)),this, SLOT(selectSales()));
    connect(ui->btnAdd_2,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->btnDel_2,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->btnEdit_2,SIGNAL(clicked(bool)),this, SLOT(edit()));
    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));

    ui->twOrg->setColumnCount(5);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("id"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("date"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("product_id"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("quantity"));
    ui->twOrg->setHorizontalHeaderItem(4,new QTableWidgetItem("total_price"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Sales::~Sales()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}

void Sales::dbconnect()
{
    if(!dbconn.isOpen())
    {
        dbconn=QSqlDatabase::database("main_connection");

    }
}

void Sales::selectSales()
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
    QString sqlstr = "select * from Sales";
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
        ui->twOrg->setItem(i,1,new QTableWidgetItem(query.value("date").toString()));
        ui->twOrg->setItem(i,2,new QTableWidgetItem(query.value("product_id").toString()));
        ui->twOrg->setItem(i,3,new QTableWidgetItem(query.value("quantity").toString()));
        ui->twOrg->setItem(i,4,new QTableWidgetItem(query.value("total_price").toString()));
        i++;
    }
}

void Sales::add()
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
    QString sqlstr = "insert into Sales (id, date, product_id, quantity, total_price) values(?,?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->leAbbr->text().isEmpty() or ui->teTitle->text().isEmpty() or ui->leCity->text().isEmpty() or ui->lineEdit->text().isEmpty() or ui->lineEdit_2->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поля перед тем как добавлять строчку!");
        return;
    }
    QRegularExpression dateRegex("\\d{4}-\\d{2}-\\d{2}"); // задание регулярного выражения для формата даты
    QRegularExpressionMatch match = dateRegex.match(ui->teTitle->text()); // проверка соответствия формату даты
    if (!match.hasMatch()) {
        QMessageBox::critical(this,"Error","Нужно вводить дату формата xxxx-xx-xx !");
        return;
    }
    if (ui->leAbbr->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле id не может быть меньше 1!");
        return;
    }
    if (ui->leCity->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле product_id не может быть меньше 1!");
        return;
    }
    if (ui->lineEdit->text().toInt() < 0) {
        QMessageBox::critical(this,"Error","Поле quantity не может быть меньше 0!");
        return;
    }
    if (ui->lineEdit_2->text().toFloat() < 0) {
        QMessageBox::critical(this,"Error","Поле total_price не может быть меньше 0!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leAbbr->text().toInt());
    query.bindValue(1,ui->teTitle->text());
    query.bindValue(2,ui->leCity->text().toInt());
    query.bindValue(3,ui->lineEdit->text().toInt());
    query.bindValue(4,ui->lineEdit_2->text().toFloat());
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
    selectSales();
}

void Sales::del()
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
    QString sqlstr = "delete from Sales where id = '" + ui->twOrg->item(currow,0)->text() + "'";
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
    selectSales();
}

void Sales::edit()
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
    QString sqlstr = "UPDATE Sales SET id = ?, date = ?, product_id = ?, quantity = ?, total_price = ? WHERE id ='" + ui->twOrg->item(currow,0)->text() + "'";;
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->leAbbr->text().isEmpty() or ui->teTitle->text().isEmpty() or ui->leCity->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поля перед тем как редактировать строчку!");
        return;
    }
    QRegularExpression dateRegex("\\d{4}-\\d{2}-\\d{2}"); // задание регулярного выражения для формата даты
    QRegularExpressionMatch match = dateRegex.match(ui->teTitle->text()); // проверка соответствия формату даты
    if (!match.hasMatch()) {
        QMessageBox::critical(this,"Error","Нужно вводить дату формата xxxx-xx-xx !");
        return;
    }
    if (ui->leAbbr->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле id не может быть меньше 1!");
        return;
    }
    if (ui->leCity->text().toInt() < 1) {
        QMessageBox::critical(this,"Error","Поле product_id не может быть меньше 1!");
        return;
    }
    if (ui->lineEdit->text().toInt() < 0) {
        QMessageBox::critical(this,"Error","Поле quantity не может быть меньше 0!");
        return;
    }
    if (ui->lineEdit_2->text().toFloat() < 0) {
        QMessageBox::critical(this,"Error","Поле total_price не может быть меньше 0!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leAbbr->text().toInt());
    query.bindValue(1,ui->teTitle->text());
    query.bindValue(2,ui->leCity->text().toInt());
    query.bindValue(3,ui->lineEdit->text().toInt());
    query.bindValue(4,ui->lineEdit_2->text().toFloat());
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
    selectSales();

}


