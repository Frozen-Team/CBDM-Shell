#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T11:25:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#ROOT_DIRECTORY = $$PWD
#LIB_DIRECTORY = $${ROOT_DIRECTORY}/lib
#BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
#TESTS_DIRECTORY = $${BUILD_DIRECTORY}/tests
#EXAMPLES_DIRECTORY = $${BUILD_DIRECTORY}/examples
#EXEC_DIRECTORY = $${BUILD_DIRECTORY}

contains(QMAKE_TARGET.arch, x86_64):TARGET = GRW
!contains(QMAKE_TARGET.arch, x86_64):TARGET = GRW86

TEMPLATE = app

RC_FILE = resources.rc

RESOURCES += \
    res.qrc

SOURCES += \
    main.cpp \
    dependencymanagermain.cpp \
    settingsdialog.cpp \
    helpdialog.cpp \
    modulesettings.cpp \
    configurableui.cpp \
    genprocess.cpp

HEADERS  += \
    version.h \
    dependencymanagermain.h \
    settingsdialog.h \
    helpdialog.h \
    modulesettings.h \
    configurableui.h \
    qtutils.h \
    genprocess.h

DISTFILES += \
    resources.rc

MOC_DIR = temp/
OBJECTS_DIR = temp/
RCC_DIR = temp/


## Destination configs
## release build
Release: DESTDIR = bin
## debug build
Debug: DESTDIR = bind

message($${DESTDIR})
#!debug {
#    # Make folders and copy styles post build
#    # Windows
#    win32
#    {
#        system(windeployqt $${PWD}\Build-32bit-Release\release)
#        system(windeployqt $${PWD}\Build-64bit-Release\release)

#        DESTDIR_WIN = $${DESTDIR}
#        DESTDIR_WIN ~= s,/,\\,g
#        PWD_WIN = $${PWD}
#        PWD_WIN ~= s,/,\\,g
#        for(FILE, DISTFILES)
#        {
#            #QMAKE_POST_LINK += copy $${PWD_WIN}\\$${FILE} $${PWD_WIN}\\Build-32bit-Debug\\$${FILE}
#        }

#    }

#    # Linux
#    unix
#    {
#        for(FILE, DISTFILES)
#        {
#            #QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${DESTDIR}$$escape_expand(\\n\\t))
#        }
#    }

#    # Mac os
#    unix:!macx
#    {

#    }
#}



FORMS += \
    dependencymanagermain.ui \
    settingsdialog.ui \
    helpdialog.ui \
    modulesettings.ui


