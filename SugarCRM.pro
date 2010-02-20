# -------------------------------------------------
# Project created by QtCreator 2010-01-14T17:49:12
# -------------------------------------------------
include(qtsoap/src/qtsoap.pri)
QT += webkit
RC_FILE = app.rc
TARGET = SugarCRM
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    sugarcrmsoap.cpp \
    logindialog.cpp \
    sugarcrm.cpp \
    dockwidget.cpp \
    loadingdialog.cpp \
    accountmodel.cpp \
    account.cpp \
    accountproxymodel.cpp \
    sugarsettings.cpp \
    accountdetail.cpp \
    note.cpp \
    notesmodel.cpp \
    createnotedialog.cpp \
    settingsdialog.cpp
HEADERS += mainwindow.h \
    sugarcrmsoap.h \
    logindialog.h \
    sugarcrm.h \
    dockwidget.h \
    loadingdialog.h \
    accountmodel.h \
    account.h \
    accountproxymodel.h \
    sugarsettings.h \
    accountdetail.h \
    note.h \
    notesmodel.h \
    createnotedialog.h \
    settingsdialog.h
FORMS += mainwindow.ui
RESOURCES += app.qrc
OTHER_FILES += SugarCrm.qml \
    content/AccountView.qml \
    content/AuthView.qml \
    content/TestListModel.qml \
    content/PropertyEdit.qml \
    content/Button.qml \
    content/SearchField.qml
CODECFORTR      = UTF-8
CODECFORSRC     = UTF-8
