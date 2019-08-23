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


SOURCES += src/main.cpp\
		src/mainwindow/mainwindow.cpp \
	src/editor/codeeditor.cpp \
	src/app/helpers.cpp \
	src/dialogs/newprojectwizard.cpp \
	src/dialogs/newprojectwizardfirstpage.cpp \
	src/dialogs/newprojectwizarddefaultpage.cpp \
	src/dialogs/newxmldialog.cpp \
	src/xml/xmlfilehandler.cpp \
	src/projects/projecthandler.cpp \
	src/dialogs/importfiledialog.cpp \
	src/xml-validation/schemavalidation.cpp \
	src/xml-validation/messagehandler.cpp \
	src/editor/highlighter.cpp \
	src/projects/projectitemmodel.cpp \
	src/dialogs/openprojectfiledialog.cpp \
	src/projects/projectlistitem.cpp \
	src/elbe/elbehandler.cpp \
	src/elbe/buildprocess.cpp \
	src/elbe/buildprocessworker.cpp \
	src/dialogs/buildprocessstartdialog.cpp \
	src/dialogs/filedownloaddialog.cpp \
	src/elbe/buildmanager.cpp \
	src/elbe/buildprocessstatusbarupdate.cpp \
	src/dialogs/elbesettingsdialog.cpp \
	src/projects/existingprojects.cpp \
	src/app/applicationconfig.cpp \
	src/dialogs/deletedialog.cpp \
	src/xml/xmlfile.cpp \
	src/projects/project.cpp \
	src/projects/projectproperties.cpp \
	src/xml/xmlutilities.cpp \
	src/app/filesystemwatcher.cpp \
    src/update/updates.cpp \
    src/update/updatetab.cpp \
    src/update/updatelistitem.cpp \
    src/update/updatethread.cpp \
    src/update/checkupdatethread.cpp \
    src/dialogs/changeworkspacedialog.cpp \
    src/dialogs/createinitvmdialog.cpp

HEADERS  += src/mainwindow/mainwindow.h \
	src/editor/codeeditor.h \
	src/app/helpers.h \
	src/dialogs/newprojectwizard.h \
	src/dialogs/newprojectwizardfirstpage.h \
	src/dialogs/newprojectwizarddefaultpage.h \
	src/dialogs/newxmldialog.h \
	src/xml/xmlfilehandler.h \
	src/projects/projecthandler.h \
	src/dialogs/importfiledialog.h \
	src/xml-validation/schemavalidation.h \
	src/xml-validation/messagehandler.h \
	src/editor/highlighter.h \
	src/projects/projectitemmodel.h \
	src/dialogs/openprojectfiledialog.h \
	src/projects/projectlistitem.h \
	src/elbe/elbehandler.h \
	src/elbe/buildprocess.h \
	src/elbe/buildprocessworker.h \
	src/dialogs/buildprocessstartdialog.h \
	src/dialogs/filedownloaddialog.h \
	src/elbe/buildmanager.h \
	src/elbe/buildprocessstatusbarupdate.h \
	src/dialogs/elbesettingsdialog.h \
	src/projects/existingprojects.h \
	src/app/applicationconfig.h \
	src/dialogs/deletedialog.h \
	src/xml/xmlfile.h \
	src/projects/project.h \
	src/projects/projectproperties.h \
	src/xml/xmlutilities.h \
	src/app/filesystemwatcher.h \
    src/update/updates.h \
    src/update/updatetab.h \
    src/update/updatelistitem.h \
    src/update/updatethread.h \
    src/update/checkupdatethread.h \
    src/dialogs/changeworkspacedialog.h \
    src/dialogs/createinitvmdialog.h

FORMS    += src/mainwindow/mainwindow.ui \
	src/dialogs/newprojectwizard.ui \
	src/dialogs/newxmldialog.ui \
	src/dialogs/importfiledialog.ui \
	src/dialogs/buildprocessstartdialog.ui \
	src/dialogs/filedownloaddialog.ui \
	src/dialogs/elbesettingsdialog.ui \
	src/dialogs/deletedialog.ui \
    src/update/updatetab.ui \
    src/dialogs/changeworkspacedialog.ui \
    src/dialogs/createinitvmdialog.ui

RESOURCES += style/style.qrc \
	templates/filetemplates.qrc \

#unix|win32: LIBS += -lqtermwidget5

DISTFILES +=

unix:!macx: LIBS += -lyaml-cpp
