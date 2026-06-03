QT += core network sql

CONFIG += console c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = Tema8CppServer

SOURCES += \
    main.cpp \
    mytcpserver.cpp \
    database.cpp \
    algorithms.cpp

HEADERS += \
    mytcpserver.h \
    database.h \
    algorithms.h
