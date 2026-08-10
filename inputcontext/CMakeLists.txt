############################################################################
# QSkinny - Copyright (C) The authors
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

set(SOURCES QskInputContextPlugin.cpp)

set(target qskinputcontext)

qsk_add_plugin(${target} platforminputcontexts QskInputContextPlugin ${SOURCES})

target_link_libraries(${target} PRIVATE qskinny Qt::GuiPrivate)
