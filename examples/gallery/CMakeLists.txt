############################################################################
# QSkinny - Copyright (C) The authors
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

set(SOURCES
    inputs/InputPage.h inputs/InputPage.cpp
    progressbar/ProgressBarPage.h progressbar/ProgressBarPage.cpp
    button/ButtonPage.h button/ButtonPage.cpp
    selector/SelectorPage.h selector/SelectorPage.cpp
    dialog/DialogPage.h dialog/DialogPage.cpp
    listbox/ListBoxPage.h listbox/ListBoxPage.cpp

    colorpickers/ColorPickerPage.cpp
    colorpickers/ColorPickerPage.h
    colorpickers/QskColorRectangle.cpp
    colorpickers/QskColorRectangle.h
    colorpickers/QskColorRectangleSkinlet.cpp
    colorpickers/QskColorRectangleSkinlet.h
    colorpickers/QskColorTriangle.cpp
    colorpickers/QskColorTriangle.h
    colorpickers/QskColorTriangleNode.cpp
    colorpickers/QskColorTriangleNode.h
    colorpickers/QskColorTriangleSkinlet.cpp
    colorpickers/QskColorTriangleSkinlet.h
    colorpickers/QskColorWheel.cpp
    colorpickers/QskColorWheel.h
    colorpickers/QskColorWheelSkinlet.cpp
    colorpickers/QskColorWheelSkinlet.h
    

    Page.h Page.cpp
    main.cpp
)
qt_add_resources(SOURCES icons.qrc)

qsk_add_example(gallery ${SOURCES})

