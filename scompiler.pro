#-------------------------------------------------
#
# Project created by QtCreator 2011-11-28T22:55:59
#
#-------------------------------------------------

QT     += core gui
CONFIG += qt warn_on debug

TARGET = scompiler
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    slexer.cpp

HEADERS += mainwindow.h \
    slexer.h

FORMS += mainwindow.ui

RESOURCES += scompiler.qrc

RC_FILE = scompiler.rc
