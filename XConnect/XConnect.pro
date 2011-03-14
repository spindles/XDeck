# -------------------------------------------------
# Project created by QtCreator 2010-02-24T14:03:09
# -------------------------------------------------
QT += network \
    opengl \
    sql \
    svg \
    webkit \
    gui
TARGET = XConnect
TEMPLATE = app
SOURCES += main.cpp \
    charactersheet.cpp \
    mainwindow.cpp \
    blackmask.cpp \
    cloak.cpp \
    darcknight.cpp \
    flatline.cpp \
    genie.cpp \
    guarddog.cpp \
    hammer.cpp \
    hellhound.cpp \
    hydra.cpp \
    lightningbug.cpp \
    reaper.cpp \
    shield.cpp \
    wizardbook.cpp
HEADERS += mainwindow.h \
    charactersheet.h \
    blackmask.h \
    cloak.h \
    darcknight.h \
    flatline.h \
    genie.h \
    guarddog.h \
    hammer.h \
    hellhound.h \
    hydra.h \
    lightningbug.h \
    reaper.h \
    shield.h \
    wizardbook.h
FORMS += mainwindow.ui \
    blackmask.ui \
    cloak.ui \
    darcknight.ui \
    flatline.ui \
    genie.ui \
    guarddog.ui \
    hammer.ui \
    hellhound.ui \
    hydra.ui \
    lightningbug.ui \
    reaper.ui \
    shield.ui \
    wizardbook.ui
RESOURCES += images.qrc
OTHER_FILES += ATTRIBUTIONS.txt \
    README.txt
LIBS += -lFestival \
    -lestools \
    -lestbase \
    -leststring \
    -lncurses \
    -lasound
