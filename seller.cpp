#include "seller.h"
#include "ui_seller.h"
#include <iostream>

#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>
#include <QComboBox>

void Seller::showBack() {

    this->close();
    clearCombo();
    emit formsWindow();

}

void Seller::showExit() {
    this->close();
}

void Seller::showCustomers() {
    windowCustomers->show();
    this->close();
    //emit sellersWindow();
}

Seller::Seller(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Seller)
{
    ui->setupUi(this);
    windowCustomers = new Customer();
    connect(windowCustomers, &Customer::sellersWindow, this, &Seller::show);
    //connect(ui->btnCustomers,SIGNAL(clicked(bool)),this, SLOT(showCustomers()));
    connect(ui->btnSales,SIGNAL(clicked(bool)),this, SLOT(selectSellers()));
    connect(ui->btnAdd_2,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->btnDel_2,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->btnEdit_2,SIGNAL(clicked(bool)),this, SLOT(edit()));
    connect(ui->btnBack,SIGNAL(clicked(bool)),this, SLOT(showBack()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this, SLOT(showExit()));
    connect(ui->btnSearch,SIGNAL(clicked(bool)),this, SLOT(search()));
    //connect(ui->comboBoxShopAddress, SIGNAL(activated(int)), this, SLOT(handleSelection(int)));    //setCombo();
    ui->twOrg->setColumnCount(5);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0, new QTableWidgetItem("adress"));
    ui->twOrg->setHorizontalHeaderItem(1, new QTableWidgetItem("Full name"));
    ui->twOrg->setHorizontalHeaderItem(2, new QTableWidgetItem("post"));
    ui->twOrg->setHorizontalHeaderItem(3, new QTableWidgetItem("phone"));
    ui->twOrg->setHorizontalHeaderItem(4, new QTableWidgetItem("salary"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    //ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Seller::~Seller()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}

void Seller::dbconnect()
{
    if(!dbconn.isOpen())
    {
        dbconn=QSqlDatabase::database("main_connection");

    }

}

void Seller::handleSelection(int index)
{
    QString selectedAddress = ui->comboBoxShopAddress->itemText(index);
    // Perform any necessary actions with the selected address
}


void Seller::getId() {
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
    QStringList shopAddresses;  // Список для хранения адресов магазинов
    QSqlQuery query(dbconn);

    // Шаг 2: Обработка нажатия кнопки добавления пользователя
    QString selectedAddress = ui->comboBoxShopAddress->currentText();

    // Шаг 3: Получение ID магазина на основе выбранного адрес
    QString shopIdQueryStr = "SELECT shop_id FROM shops WHERE adress = :adress";
    QSqlQuery shopIdQuery(dbconn);
    shopIdQuery.prepare(shopIdQueryStr);
    shopIdQuery.bindValue(":adress", selectedAddress);
    if (shopIdQuery.exec() && shopIdQuery.next()) {
        selectedShopIndex = shopIdQuery.value("shop_id").toInt();
    }
}

void Seller::selectSellers()
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
    QString sqlstr = "SELECT s.adress, w.full_name, w.post, w.phone, w.salary "
                     "FROM workers w "
                     "JOIN shops s ON w.work_place_id = s.shop_id";
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
    while (query.next())
    {
        ui->twOrg->setItem(i, 0, new QTableWidgetItem(query.value("adress").toString()));
        ui->twOrg->setItem(i, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->twOrg->setItem(i, 2, new QTableWidgetItem(query.value("post").toString()));
        ui->twOrg->setItem(i, 3, new QTableWidgetItem(query.value("phone").toString()));
        ui->twOrg->setItem(i, 4, new QTableWidgetItem(query.value("salary").toString()));
        i++;
    }
}

void Seller::add()
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

    QString sqlstr;
    QSqlQuery query(dbconn);
    // Создать строку запроса2
    sqlstr = "insert into workers (work_place_id, full_name, post, phone, salary) values(?,?,?,?,?)";
    // Подготовить запрос

    query.prepare(sqlstr);
    if (ui->leName->text().isEmpty() or ui->tePost->text().isEmpty() or ui->lephone->text().isEmpty() or ui->lineEdit_2->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поля перед тем как добавлять строчку!");
        return;
    }
    // Передать параметры из полей ввода в запрос
    getId();
    query.bindValue(0, selectedShopIndex);
    query.bindValue(1 ,ui->leName->text());
    query.bindValue(2, ui->tePost->text());
    query.bindValue(3, ui->lephone->text());
    query.bindValue(4, ui->lineEdit_2->text());
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
    selectSellers();
}

void Seller::del()
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
    QString sqlstr = "delete from workers where full_name = '" + ui->twOrg->item(currow,1)->text() + "'";
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
    //selectSellers();
}

void Seller::edit()
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
    // work_place_id, full_name, post, phone, salary
    QString sqlstr = "UPDATE workers SET work_place_id = ?, full_name = ?, post = ?, phone = ?, salary = ? WHERE full_name ='" + ui->twOrg->item(currow,1)->text() + "'";;
    // Подготовить запрос
    query.prepare(sqlstr);
    if (ui->leName->text().isEmpty() or ui->tePost->text().isEmpty() or ui->lephone->text().isEmpty() or ui->lineEdit_2->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поля перед тем как редактировать строчку!");
        return;
    }
\
    // Передать параметры из полей ввода в запрос
    getId();
    query.bindValue(0, selectedShopIndex);
    query.bindValue(1 ,ui->leName->text());
    query.bindValue(2,ui->tePost->text());
    query.bindValue(3,ui->lephone->text());
    query.bindValue(4,ui->lineEdit_2->text());

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
    //selectSellers();

}

void Seller::search() {

    ui->twOrg->clearContents();

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
    QString sqlstr = "SELECT s.adress, w.full_name, w.post, w.phone, w.salary "
                     "FROM workers w "
                     "JOIN shops s ON w.work_place_id = s.shop_id "
                     "WHERE w.full_name = :full_name";

    query.prepare(sqlstr);

    if (ui->lineSearch->text().isEmpty()) {
        QMessageBox::critical(this,"Error","Заполните поле перед тем как начинать поиск!");
        return;
    }
    QString selectedName = ui->lineSearch->text();
    query.bindValue(":full_name", selectedName);
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
    while (query.next())
    {
        ui->twOrg->setItem(i, 0, new QTableWidgetItem(query.value("adress").toString()));
        ui->twOrg->setItem(i, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->twOrg->setItem(i, 2, new QTableWidgetItem(query.value("post").toString()));
        ui->twOrg->setItem(i, 3, new QTableWidgetItem(query.value("phone").toString()));
        ui->twOrg->setItem(i, 4, new QTableWidgetItem(query.value("salary").toString()));
        i++;
    }
}

void Seller::setCombo() {

    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    QStringList shopAddresses;  // Список для хранения адресов магазинов
    QSqlQuery query(dbconn);
    QString sqlstr = "SELECT adress FROM shops";
    if (query.exec(sqlstr)) {
        while (query.next()) {
            QString address = query.value("adress").toString();
            if (!address.isEmpty()) {
                shopAddresses << address;
            }
        }
    }
    ui->comboBoxShopAddress->addItems(shopAddresses);
}

void Seller::clearCombo() {
    ui->comboBoxShopAddress->clear();
}
