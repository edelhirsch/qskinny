############################################################################
# QSkinny - Copyright (C) The authors
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

if(QT_VERSION_MAJOR VERSION_LESS 6)
    find_package(Qt5 REQUIRED COMPONENTS Test)
else()
    find_package(Qt6 REQUIRED COMPONENTS Test)
endif()

add_subdirectory(auto)
