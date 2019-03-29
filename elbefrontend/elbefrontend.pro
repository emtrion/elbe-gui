#-------------------------------------------------
#
# Project created by QtCreator 2019-01-14T14:36:42
#
#-------------------------------------------------

QT       += core gui
QT		 += xml
QT		 += xmlpatterns

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
    helpers.cpp \
    newprojectwizard.cpp \
    newprojectwizardfirstpage.cpp \
    newprojectwizarddefaultpage.cpp \
    newxmldialog.cpp \
    xmlfilehandler.cpp \
    projecthandler.cpp \
    importfiledialog.cpp \
    schemavalidation.cpp \
    messagehandler.cpp \
    highlighter.cpp \
    projectitemmodel.cpp \
    openprojectfiledialog.cpp \
    projectlistitem.cpp \
    elbehandler.cpp \
    buildprocess.cpp \
    buildprocessworker.cpp \
    buildprocessstartdialog.cpp \
    filedownloaddialog.cpp \
    buildmanager.cpp \
    buildprocessstatusbarupdate.cpp \
    elbesettingsdialog.cpp \
    existingprojects.cpp \
    applicationconfig.cpp \
    deletedialog.cpp \
    xmlfile.cpp \
    project.cpp \
    projectproperties.cpp \
    xmlutilities.cpp \
    filesystemwatcher.cpp

HEADERS  += mainwindow.h \
    codeeditor.h \
    helpers.h \
    newprojectwizard.h \
    newprojectwizardfirstpage.h \
    newprojectwizarddefaultpage.h \
    newxmldialog.h \
    xmlfilehandler.h \
    projecthandler.h \
    importfiledialog.h \
    schemavalidation.h \
    messagehandler.h \
    highlighter.h \
    projectitemmodel.h \
    openprojectfiledialog.h \
    projectlistitem.h \
    elbehandler.h \
    buildprocess.h \
    buildprocessworker.h \
    buildprocessstartdialog.h \
    filedownloaddialog.h \
    buildmanager.h \
    buildprocessstatusbarupdate.h \
    elbesettingsdialog.h \
    existingprojects.h \
    applicationconfig.h \
    deletedialog.h \
    xmlfile.h \
    project.h \
    projectproperties.h \
    xmlutilities.h \
    filesystemwatcher.h

FORMS    += mainwindow.ui \
    newprojectwizard.ui \
    newxmldialog.ui \
    importfiledialog.ui \
    buildprocessstartdialog.ui \
    filedownloaddialog.ui \
    elbesettingsdialog.ui \
    deletedialog.ui

RESOURCES += style.qrc \
    filetemplates.qrc

unix|win32: LIBS += -lqtermwidget5

DISTFILES +=


unix:!macx: LIBS += -lyaml-cpp
