TEMPLATE = app
TARGET = UWW SIMULATOR

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    uwcpu.cpp \
    ram.cpp

HEADERS += \
    uwcpu.h \
    ram.h
