#!/bin/bash

set -e

OUTDIR=website/content
PUBDIR=/var/www/html/
DOXYBOOK=~/dev/doxybook2/build/src/DoxybookCli/doxybook2
WEBSITE=~/dev/edelhirsch.github.io

cd "$(dirname "$0")"

# generate XML if necessary:
[ ! -d "xml" ] && doxygen

# We need our own version of doxybook2:
if [ ! -d "$DOXYBOOK" ]
then
    mkdir -p dev
    cd ~/dev
    git clone git@github.com:edelhirsch/doxybook2.git
    vcpkg install --triplet x64-linux $(cat vcpkg.txt)
    mkdir build
    cmake -B ./build -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=MinSizeRel \
        -DCMAKE_TOOLCHAIN_FILE=/usr/local/share/vcpkg/scripts/buildsystems/vcpkg.cmake ..
    cmake --build ./build --config MinSizeRel
fi

# generate Markdown from XML:
$DOXYBOOK -i xml -o ~/dev/qskinny-website/docs -c doxybook2-config.json

# generate static HTML from Markdown:
cd ~/dev/qskinny-website
bundle exec jekyll build

# copy static HTML to website:
cd ~/dev
[ ! -d "$WEBSITE" ] && git clone git@github.com:edelhirsch/edelhirsch.github.io.git
cd edelhirsch.github.io/
#rm -r ./*
cp -r ~/dev/qskinny-website/_site/* ./
git add ./*
git commit -m "new version"

echo
echo "Done. Now you have to push the new version manually"
