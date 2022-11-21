# -------------------------------------------------
# Project created by QtCreator 2011-03-04T13:38:06
# -------------------------------------------------
QT += opengl \
	widgets
QT -= gui
TARGET = ../ariface
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += object.cpp \
           main.cpp
HEADERS += object.h

LIBS += -L/usr/lib/artoolkit -lARgsub \
    -L/usr/lib/artoolkit -lARvideo \
    -L/usr/lib/artiilkit -lAR \
    -lpthread \
    -lglut \
    -lGLU \
    -lGL \
    -lXi \
    -lX11 \
    -lm
