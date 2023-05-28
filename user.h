#ifndef USER_H
#define USER_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class User;
}

class User : public QDialog
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr);
    ~User();

signals:
    void mainWindow();

private:
    Ui::User *ui;
    QSqlDatabase dbconn;

public slots:
    void dbconnect();
    void showExit();
    void showBack();
    void selectProducts();
};

#endif // USER_H
