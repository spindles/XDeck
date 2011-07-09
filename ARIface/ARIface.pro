# -------------------------------------------------
# Project created by QtCreator 2011-03-04T13:38:06
# -------------------------------------------------
QT += opengl
QT -= gui
TARGET = ../ariface
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += object.cpp \
           main.cpp
HEADERS += object.h

LIBS += -lARgsub \
    -lARvideo \
    -lAR \
    -lpthread \
    -lglut \
    -lGLU \
    -lGL \
    -lXi \
    -lX11 \
    -lm
