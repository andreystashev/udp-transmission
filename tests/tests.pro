QT += core network testlib
QT -= gui
CONFIG += c++17 testcase console
TEMPLATE = app
TARGET = udp-tests
SOURCES += tst_udp.cpp \
    ../data-sender/udpsender.cpp \
    ../data-receiver/udpreceiver.cpp
HEADERS += ../data-sender/udpsender.h \
    ../data-receiver/udpreceiver.h \
    ../common/data.h
