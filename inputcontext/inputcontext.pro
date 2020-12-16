TEMPLATE = lib
TARGET = $$qskPluginTarget(qskinputcontext)

QT += gui-private

CONFIG += plugin
CONFIG += qskinny

QSK_PLUGIN_SUBDIR = platforminputcontexts
contains(QSK_CONFIG, QskDll): DEFINES += QSK_INPUTCONTEXT_MAKEDLL

HEADERS += \
    QskInputContextGlobal.h

SOURCES += \
    QskInputContextPlugin.cpp

OTHER_FILES += metadata.json

target.path    = $${QSK_INSTALL_PLUGINS}/$${QSK_PLUGIN_SUBDIR}
INSTALLS       = target
