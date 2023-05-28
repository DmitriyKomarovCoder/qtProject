#ifndef OTCHET_H
#define OTCHET_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class Otchet;
}

class Otchet : public QDialog
{
    Q_OBJECT

public:
    explicit Otchet(QWidget *parent = nullptr);
    ~Otchet();

signals:
    void adminWindow();

private:
    Ui::Otchet *ui;
    QSqlDatabase dbconn;

public slots:
    void dbconnect();
    void showBack();
    void showExit();
    void selectCustomers();
    void selectSellers();
    void selectSum10();

};

#endif // OTCHET_H
