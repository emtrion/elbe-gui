#-------------------------------------------------
#
# Project created by QtCreator 2019-01-14T14:36:42
#
#-------------------------------------------------

QT       += core gui
QT		 += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = elbefrontend
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    codeeditor.cpp \
    userdata.cpp \
    helpers.cpp \
    newprojectwizard.cpp \
    newprojectwizardfirstpage.cpp \
    newprojectwizarddefaultpage.cpp \
    newxmldialog.cpp \
    xmlfilehandler.cpp \
    projectmanager.cpp \
    projecthandler.cpp

HEADERS  += mainwindow.h \
    codeeditor.h \
    userdata.h \
    helpers.h \
    newprojectwizard.h \
    newprojectwizardfirstpage.h \
    newprojectwizarddefaultpage.h \
    newxmldialog.h \
    xmlfilehandler.h \
    projectmanager.h \
    projecthandler.h

FORMS    += mainwindow.ui \
    newprojectwizard.ui \
    newxmldialog.ui

RESOURCES += style.qrc \
    filetemplates.qrc

unix|win32: LIBS += -lqtermwidget5

DISTFILES +=

