#!/bin/bash

OUTDIR=website/content
PUBDIR=/var/www/html/

cd "$(dirname "$0")"

# generate XML if necessary:
[ ! -d "xml" ] && doxygen

# generate Markdown from XML:
mkdir -p $OUTDIR
doxybook2 -i xml -o $OUTDIR -c website/config.json

# generate static HTML from Markdown:
cd website
[ ! -d "themes/hugo-book" ] && git clone https://github.com/alex-shpak/hugo-book.git themes/hugo-book
hugo

cp -r public $PUBDIR
