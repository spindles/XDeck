SOURCES += configwizard.cpp \
    main.cpp
HEADERS += configwizard.h
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt
TARGET = configwizard
RESOURCES = application.qrc
QT += sql
OTHER_FILES += TODO.txt \
    README.txt
