#-------------------------------------------------
#
# Project created by QtCreator 2011-11-28T22:55:59
#
#-------------------------------------------------

QT     += core gui
CONFIG += qt warn_on

TARGET = scompiler
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    codeeditor.cpp \
    slexicalanalyzer.cpp \
    ssyntacticanalyzer.cpp

HEADERS += mainwindow.h \
    codeeditor.h \
    errors.h \
    basics.h \
    slexicalanalyzer.h \
    ssyntacticanalyzer.h

FORMS += mainwindow.ui

RESOURCES += scompiler.qrc

RC_FILE = scompiler.rc










