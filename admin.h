#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include "otchet.h"
#include "forms.h"
#include "zapros.h"


namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

signals:
    void mainWindow();

private:
    Ui::Admin *ui;
    Otchet* windowOtchet;
    Forms* windowForms;
    Zapros* windowZapros;

public slots:
    void showForm();
    void showOtchet();
    void showZapros();
    void showExit();
    void showBack();
};

#endif // ADMIN_H
