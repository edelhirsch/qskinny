CONFIG += qskexample

HEADERS += \
    MainWindow.h

SOURCES += \
    MainWindow.cpp \
    main.cpp

QRCFILES += \
    qvgviewer.qrc

SVGSOURCES = \
    svg/01.01.10.svg \
    svg/01.03.04q.svg \
    svg/01.08.05q.svg \
    svg/01.25.18.svg

cross_compile {
    # for now just assume that a desktop version of the tool is in the path
    SVG2QVG=svg2qvg
} else {
    # Convert path to shell path, otherwise it fails on Windows.
    SVG2QVG=$$shell_path($${QSK_OUT_ROOT}/tools/bin/svg2qvg)
}

svg2qvg.name = SVG compiler
svg2qvg.input = SVGSOURCES
svg2qvg.output = qvg/${QMAKE_FILE_BASE}.qvg
svg2qvg.variable_out = 
svg2qvg.commands += ($$sprintf($${QMAKE_MKDIR_CMD}, qvg)) && $${SVG2QVG} ${QMAKE_FILE_IN} $${svg2qvg.output}

rccgen.name = RCC compiler
rccgen.input = QRCFILES
rccgen.output = $${RCC_DIR}/qrc_${QMAKE_FILE_BASE}.cpp
rccgen.variable_out = SOURCES
rccgen.commands += ($$sprintf($${QMAKE_MKDIR_CMD}, $${RCC_DIR}))

!equals( OUT_PWD, $${PWD} ) {

    # Paths inside a qrc file are always relative to the path of the 
    # qrc file itself. So in case of shadow builds we need to copy the
    # qrc file into the shadow directory as the included qvg files
    # are generated locally.

    QRC_SHADOW_CLONE = $$shell_path($${OUT_PWD}/${QMAKE_FILE_BASE}_shadow.qrc)

    rccgen.commands += && $${QMAKE_COPY} ${QMAKE_FILE_IN} $${QRC_SHADOW_CLONE}
    # Use shell_path again to prevent from path error under Windows
    rccgen.commands += && $$shell_path($$dirname(QMAKE_QMAKE)/rcc) $${QRC_SHADOW_CLONE} -o ${QMAKE_FILE_OUT}
    rccgen.commands += && $${QMAKE_DEL_FILE} $${QRC_SHADOW_CLONE}
}
else {
    rccgen.commands += && $$dirname(QMAKE_QMAKE)/rcc ${QMAKE_FILE_IN} -o ${QMAKE_FILE_OUT}
}

# We would like to define a dependency for "svg2qvg.variable_out" -
# but how to do this ? As long as we don't have a solution we have to use
# the workaround below.

defineReplace(qvgfiles) {
    svgfiles = $$1

    myfiles=
    for(svgfile, svgfiles) myfiles += $$replace(svgfile, svg, qvg)
    return($$myfiles)
}
rccgen.depends += $$qvgfiles( $${SVGSOURCES} )

QMAKE_EXTRA_COMPILERS += svg2qvg rccgen
