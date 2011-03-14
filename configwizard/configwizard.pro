SOURCES += configwizard.cpp \
    main.cpp
HEADERS += configwizard.h
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt
TARGET = ../cwizard
RESOURCES = application.qrc
QT += sql
