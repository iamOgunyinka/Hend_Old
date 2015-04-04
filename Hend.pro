#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T10:45:11
#
#-------------------------------------------------

QT       += core gui network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hend
TEMPLATE = app
QMAKE_CXXFLAGS = -std=c++11

SOURCES += main.cpp\
    networkmanager.cpp \
    mainwindow.cpp \
    videostructure.cpp \
    videotablemodel.cpp \
    searchdialog.cpp \
    extractor.cpp \
    downloadmanager.cpp \
    videodetails.cpp

HEADERS  += networkmanager.hpp \
    mainwindow.hpp \
    videostructure.hpp \
    videotablemodel.hpp \
    searchdialog.hpp \
    extractor.hpp \
    downloadmanager.hpp \
    videodetails.hpp \
    filterfunctions.hpp

FORMS    +=

RESOURCES += \
    images/images.qrc
