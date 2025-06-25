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
    RadialTickmarksNode.h RadialTickmarksNode.cpp
    Skin.h Skin.cpp
    Speedometer.h Speedometer.cpp
    SpeedometerSkinlet.h SpeedometerSkinlet.cpp
    Switch.h Switch.cpp
    ThemeColorsTile.h ThemeColorsTile.cpp
    Tile.h Tile.cpp
    UnitConversion.h UnitConversion.cpp
    main.cpp
)

# Generate .qm files from .ts files
set(TS_FILES translations_de_DE.ts translations_en_US.ts translations_zh_CN.ts)
set(QM_FILES)

foreach(TS_FILE ${TS_FILES})
    get_filename_component(LANG ${TS_FILE} NAME_WE)
    set(QM_FILE "${CMAKE_CURRENT_BINARY_DIR}/${LANG}.qm")
    
    add_custom_command(
        OUTPUT ${QM_FILE}
        COMMAND lrelease ${CMAKE_CURRENT_SOURCE_DIR}/${TS_FILE} -qm ${QM_FILE}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${TS_FILE}
        COMMENT "Generating ${QM_FILE}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    list(APPEND QM_FILES ${QM_FILE})
endforeach()

# Create a custom target for translations
add_custom_target(translations ALL DEPENDS ${QM_FILES})

# Copy the .qrc file to the binary directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/translations.qrc.in
    ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc
    COPYONLY
)

# Add the .qrc file to the build
qt_add_resources(SOURCES icons.qrc fonts.qrc ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc)

qsk_add_example(${target} ${SOURCES})

# Ensure translations are built before the target
add_dependencies(${target} translations)

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
