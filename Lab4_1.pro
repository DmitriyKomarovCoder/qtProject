QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin.cpp \
    customer.cpp \
    dialog_products.cpp \
    forms.cpp \
    main.cpp \
    mainwindow.cpp \
    otchet.cpp \
    sales.cpp \
    seller.cpp \
    supplier.cpp \
    user.cpp \
    warehouse.cpp \
    zapros.cpp

HEADERS += \
    admin.h \
    customer.h \
    dialog_products.h \
    forms.h \
    mainwindow.h \
    otchet.h \
    sales.h \
    seller.h \
    supplier.h \
    user.h \
    warehouse.h \
    zapros.h

FORMS += \
    admin.ui \
    customer.ui \
    dialog_products.ui \
    forms.ui \
    mainwindow.ui \
    otchet.ui \
    sales.ui \
    seller.ui \
    supplier.ui \
    user.ui \
    warehouse.ui \
    zapros.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
