############################################################################
# Copyright (C) The authors
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

set(target tractorui)

set(SOURCES
    ArcShadowNode.h ArcShadowNode.cpp
    Button.h Button.cpp
    ButtonSkinlet.h ButtonSkinlet.cpp
    CabinTile.h CabinTile.cpp
    Cube.h Cube.cpp
    EngineTile.h EngineTile.cpp
    GraphicProvider.h GraphicProvider.cpp
    MainBox.h MainBox.cpp
    MainWindow.h MainWindow.cpp
    PowerLiftArc.h PowerLiftArc.cpp
    PowerLiftArcSkinlet.h PowerLiftArcSkinlet.cpp
    PowerLiftTile.h PowerLiftTile.cpp
    QuickAccessButton.h QuickAccessButton.cpp
    QuickAccessTile.h QuickAccessTile.cpp
    Skin.h Skin.cpp
    Speedometer.h Speedometer.cpp
    SpeedometerSkinlet.h SpeedometerSkinlet.cpp
    Switch.h Switch.cpp
    ThemeColorsTile.h ThemeColorsTile.cpp
    Tile.h Tile.cpp
    main.cpp
)

qt_add_resources(SOURCES icons.qrc fonts.qrc)

qsk_add_example(${target} ${SOURCES})

target_compile_definitions(${target} PRIVATE USE_SHORTCUTS=1)
if(ENABLE_ENSURE_SKINS)
    target_include_directories(${target} PRIVATE ${QSK_SOURCE_DIR}/designsystems)
    target_link_libraries(${target} PRIVATE material3skin)
endif()

if (QT_VERSION_MAJOR VERSION_LESS 6) 
    qt_add_resources(SOURCES shaders.qrc)
endif()

if (QT_VERSION_MAJOR VERSION_GREATER_EQUAL 6)
    list(APPEND SHADERS
        shaders/arcshadow-vulkan.vert
        shaders/arcshadow-vulkan.frag
    )
    qsk_add_shaders(${target} "qskArcShaders" FILES ${SHADERS} OUTPUT_TARGETS shader_target)
endif()
