#ifndef DIALOG_PRODUCTS_H
#define DIALOG_PRODUCTS_H

#include <QDialog>
#include <QSqlDatabase>



namespace Ui {
class Dialog_Products;
}

class Dialog_Products : public QDialog
{
    Q_OBJECT

public:

    explicit Dialog_Products(QWidget *parent = nullptr);
    ~Dialog_Products();

private:
    Ui::Dialog_Products *ui;
    QSqlDatabase dbconn;


signals:
    void formsWindow();

public slots:
    void selectProduct();
    void add();
    void del();
    void edit();
    void dbconnect();
    void showBack();
    void showExit();
    void selectCount();
};


#endif // DIALOG_PRODUCTS_H
