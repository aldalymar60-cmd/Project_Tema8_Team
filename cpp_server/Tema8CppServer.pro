QT += core network sql

CONFIG += console c++17
CONFIG -= app_bundle

TARGET = Tema8CppServer
TEMPLATE = app

SOURCES += \
    main.cpp \
    mytcpserver.cpp

HEADERS += \
    mytcpserver.h
