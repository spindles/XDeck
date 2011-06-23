#-------------------------------------------------
#
# Project created by QtCreator 2011-03-04T13:38:06
#
#-------------------------------------------------

QT       += opengl

QT       -= gui

TARGET = ../ariface
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

LIBS += -lAR -lARgsub -lARvideo -lglut
