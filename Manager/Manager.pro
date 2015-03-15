#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T11:25:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Manager
TEMPLATE = app

RC_FILE = resources.rc

RESOURCES += \
    res.qrc

SOURCES += main.cpp\
    dependencymanagermain.cpp \
    settingsdialog.cpp \
    helpdialog.cpp \
    modulesettings.cpp \
    configurableui.cpp

HEADERS  += \
    version.h \
    dependencymanagermain.h \
    settingsdialog.h \
    helpdialog.h \
    modulesettings.h \
    dialogutils.h \
    configurableui.h

DISTFILES += \
    resources.rc \

MOC_DIR = temp/
OBJECTS_DIR = temp/
RCC_DIR = temp/


!debug {
    # Make folders and copy styles post build
    # Windows
    win32
    {
        system(mkdir -p $${PWD}\Build-32bit-Debug\debug\Styles)
        system(mkdir -p $${PWD}\Build-32bit-Release\release\Styles)
        system(windeployqt $${PWD}\Build-32bit-Release\release)

        system(mkdir -p $${PWD}\Build-64bit-Debug\debug\Styles)
        system(mkdir -p $${PWD}\Build-64bit-Release\release\Styles)
        system(windeployqt $${PWD}\Build-64bit-Releasу\release)

        DESTDIR_WIN = $${DESTDIR}
        DESTDIR_WIN ~= s,/,\\,g
        PWD_WIN = $${PWD}
        PWD_WIN ~= s,/,\\,g
        for(FILE, DISTFILES)
        {

            #QMAKE_POST_LINK += copy $${PWD_WIN}\\$${FILE} $${PWD_WIN}\\Build-32bit-Debug\\$${FILE}

        }
        #message($${PWD})
    }

    # Linux
    unix
    {
        for(FILE, DISTFILES)
        {
            #QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${DESTDIR}$$escape_expand(\\n\\t))
        }
    }

    # Mac os
    unix:!macx
    {

    }
}



FORMS += \
    dependencymanagermain.ui \
    settingsdialog.ui \
    helpdialog.ui \
    modulesettings.ui


