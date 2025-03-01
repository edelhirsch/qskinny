############################################################################
# Copyright (C) The authors
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

set(SOURCES
    Button.h Button.cpp
    ButtonSkinlet.h ButtonSkinlet.cpp
    GraphicProvider.h GraphicProvider.cpp
    MainBox.h MainBox.cpp
    MainWindow.h MainWindow.cpp
    Skin.h Skin.cpp
    Switch.h Switch.cpp
    Tile.h Tile.cpp
    main.cpp
)
qt_add_resources(SOURCES icons.qrc fonts.qrc)

qsk_add_example(tractorui ${SOURCES})

target_compile_definitions(tractorui PRIVATE USE_SHORTCUTS=1)
if(ENABLE_ENSURE_SKINS)
    target_include_directories(tractorui PRIVATE ${QSK_SOURCE_DIR}/designsystems)
    target_link_libraries(tractorui PRIVATE material3skin)
endif()
