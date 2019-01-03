#-------------------------------------------------
#
# Project created by QtCreator 2011-11-28T22:55:59
#
#-------------------------------------------------

QT     += core gui widgets
CONFIG += qt warn_on static
QMAKE_LFLAGS_RELEASE += -static -static-libgcc

TARGET = scompiler
TEMPLATE = app

SOURCES += src\main.cpp \
    src\mainwindow.cpp \
    src\codeeditor.cpp \
    src\slexicalanalyzer.cpp \
    src\ssyntacticanalyzer.cpp \
    src\errors.cpp \
    src\basics.cpp

HEADERS += src\mainwindow.h \
    src\codeeditor.h \
    src\errors.h \
    src\basics.h \
    src\slexicalanalyzer.h \
    src\ssyntacticanalyzer.h

FORMS += mainwindow.ui
RESOURCES += scompiler.qrc
RC_FILE = scompiler.rc
