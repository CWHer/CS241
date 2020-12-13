QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    infoplot.cpp \
    main.cpp \
    mainwindow.cpp \
    spatialplot.cpp \
    timeplot.cpp \
    welcomewidget.cpp

HEADERS += \
    common.h \
    database.h \
    infoplot.h \
    mainwindow.h \
    mythread.hpp \
    spatialplot.h \
    timeplot.h \
    utils.h \
    welcomewidget.h \
    xxxplot.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
