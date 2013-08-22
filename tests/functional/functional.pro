SOURCES = \
    testorderedmap.cpp

greaterThan(QT_MAJOR_VERSION, 4) {
QT += testlib
} else {
CONFIG  += qtestlib
}

include (../../src/src.pri)
