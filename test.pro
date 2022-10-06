QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    folderselector.cpp \
    imageresize.cpp \
    visitpath.cpp \
    albumdb.cpp \
    albumwindow.cpp \
    imagewindow.cpp \
    main.cpp \
    homewindow.cpp

HEADERS += \
    folderselector.h \
    imageresize.h \
    visitpath.h \
    albumdb.h \
    albumwindow.h \
    homewindow.h \
    imagewindow.h

FORMS += \
    albumwindow.ui \
    homewindow.ui \
    imageresize.ui \
    imagewindow.ui \
    folderselector.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    background.qrc \
    sample.qrc

DEFINES += PROJECT_PATH=\"\\\"$${_PRO_FILE_PWD_}/\\\"\"
