#-------------------------------------------------
#
# Project created by QtCreator 2014-05-30T10:36:57
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViewTest
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    trackitem.cpp \
    mapgraphicsview.cpp \
    shapelib/dbfopen.c \
    shapelib/safileio.c \
    shapelib/shpopen.c \
    shapelib/shptree.c \
    plotitem.cpp

HEADERS  += dialog.h \
    trackitem.h \
    mapgraphicsview.h \
    shapelib/shapefil.h \
    plotitem.h \
    transformutil.h

FORMS    += dialog.ui
