#ifndef ZAPROS_H
#define ZAPROS_H

#include <QDialog>
#include <QSqlDatabase>


namespace Ui {
class Zapros;
}

class Zapros : public QDialog
{
    Q_OBJECT

public:
    explicit Zapros(QWidget *parent = nullptr);
    ~Zapros();

signals:
    void adminWindow();

private:
    Ui::Zapros *ui;
    QSqlDatabase dbconn;


public slots:
    void showBack();
    void showExit();
    void dbconnect();
    void selectCustomersInfo();
    void selectSalesSallers();
    void selectFioSuppliers();
    void selectSort();
    void selectSum();
    void selectCount();
};

#endif // ZAPROS_H
