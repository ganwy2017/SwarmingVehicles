#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T08:49:28
#
#-------------------------------------------------

QT       -= gui

TARGET = Command_Helper
TEMPLATE = lib

win32:TARGET_EXT += .dll

DEFINES += COMMAND_HELPER_LIBRARY

SOURCES += command_helper.cpp

HEADERS += command_helper.h\
        Command_Helper_global.h

# Unix lib Install
unix:!symbian {
    target.path = $$(FLIGHTVIEW_ROOT)/lib
    INSTALLS += target
}

# Windows lib install
lib.path    = $$(FLIGHTVIEW_ROOT)/lib
win32:CONFIG(release, debug|release):       lib.files   += release/Command_Helper.lib release/Command_Helper.dll
else:win32:CONFIG(debug, debug|release):    lib.files   += debug/Command_Helper.lib debug/Command_Helper.dll
INSTALLS += lib

#Header file copy
headers.path    = $$(FLIGHTVIEW_ROOT)/include
headers.files   += $$HEADERS
INSTALLS       += headers

INCLUDEPATH += $$(FLIGHTVIEW_ROOT)/include
