#!/bin/bash

set -e

PUBDIR=/var/www/html/
DOXYBOOK=~/dev/doxybook2/build-debug/src/DoxybookCli/doxybook2
DOXYGEN=~/dev/doxygen/build/bin/doxygen
WEBSITE=~/dev/qskinny-website/
WEBSITE_PUBLIC=~/dev/edelhirsch.github.io

cd "$(dirname "$0")"

if [ "$1" == "-local" ]
then
    mkdir -p doxybook-out
    DOXYBOOK_OUT=doxybook-out
else
    DOXYBOOK_OUT=~/dev/qskinny-website/docs
fi

DOXYGEN_RUN=false

# generate HTML if necessary (to generate the inheritance diagrams):
if [ ! -d "html" ]
then
    cd ..
    git grep -i qsk_qt_private_begin|cut -d ':' -f 1|grep -v generate-website.sh|grep -v QskGlobal.h|xargs sed -i 's/QSK_QT_PRIVATE_BEGIN//'
    git grep -i qsk_qt_private_end|cut -d ':' -f 1|grep -v generate-website.sh|grep -v QskGlobal.h|xargs sed -i 's/QSK_QT_PRIVATE_END//'
    cd doc
    echo generating HTML from Doxygen for the SVGs
    $DOXYGEN Doxyfile.generate-html
    # correct links manually:
    find html/ -name 'class*.svg'|xargs sed -i 's/\.html//g; s/href=\"class/href=\"..\/class/g'
    DOXYGEN_RUN=true
fi

cp html/class*.svg $WEBSITE/docs/classes/svg/

# generate XML if necessary:
### we also need a new version of doxygen
if [ ! -d "xml" ]
then
    cd ..
    git grep -i qsk_qt_private_begin|cut -d ':' -f 1|grep -v generate-website.sh|grep -v QskGlobal.h|xargs sed -i 's/QSK_QT_PRIVATE_BEGIN//'
    git grep -i qsk_qt_private_end|cut -d ':' -f 1|grep -v generate-website.sh|grep -v QskGlobal.h|xargs sed -i 's/QSK_QT_PRIVATE_END//'
    cd doc
    echo generating XML from Doxygen
    $DOXYGEN
    DOXYGEN_RUN=true
fi

# We need our own version of doxybook2:
if [ ! -f "$DOXYBOOK" ]
then
    mkdir -p dev
    cd ~/dev
    git clone git@github.com:edelhirsch/doxybook2.git
    vcpkg install --triplet x64-linux $(cat vcpkg.txt)
    mkdir build
    cmake -B ./build -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_TOOLCHAIN_FILE=/usr/local/share/vcpkg/scripts/buildsystems/vcpkg.cmake ..
    cmake --build ./build --config Debug
fi

# generate Markdown from XML:
$DOXYBOOK -i xml -o $DOXYBOOK_OUT -c doxybook2-config.json
./generate-json-index.sh|sed -z 's/,\n]/\n]/' > $DOXYBOOK_OUT/class-index.json

if [ "$1" != "-local" ]
then
    cp -r tutorials $WEBSITE/docs/
fi

if [ $DOXYGEN_RUN = true ]
then
    git reset --hard
fi

if [ "$1" == "-generate" ]
then
    # generate static HTML from Markdown:
    cd ~/dev/qskinny-website
    bundle exec jekyll build

    # copy static HTML to website:
    cd ~/dev
    [ ! -d "$WEBSITE_PUBLIC" ] && git clone git@github.com:edelhirsch/edelhirsch.github.io.git
    cd edelhirsch.github.io/
    git rm -r ./*
    cp -r ~/dev/qskinny-website/_site/* ./
    git add ./*
    git commit -m "new version"
    git push

    echo
    echo "Done."
fi
