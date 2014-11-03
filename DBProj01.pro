#-------------------------------------------------
#
# Project created by QtCreator 2014-10-09T09:29:53
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBProj01
TEMPLATE = app

SOURCES += main.cpp\
        homewindow.cpp \
    dbconnector.cpp \
    dbquery.cpp \
    datein.cpp \
    finecal.cpp \
    editcell.cpp

HEADERS  += homewindow.h \
    dbconnector.h \
    dbquery.h \
    datein.h \
    finecal.h \
    editcell.h

FORMS    += homewindow.ui \
    dbconnector.ui \
    datein.ui \
    editcell.ui

OTHER_FILES += \
    TODO.txt \
    dbscript.txt
