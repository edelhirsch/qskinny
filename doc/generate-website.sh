#!/bin/bash

OUTDIR=website/content
PUBDIR=/var/www/html/

cd "$(dirname "$0")"

# generate XML if necessary:
[ ! -d "xml" ] && doxygen

# generate hugo site if necessary:
[ ! -d "website" ] && hugo new site website
[ ! -d "website/themes/hugo-book" ] && git clone https://github.com/alex-shpak/hugo-book.git website/themes/hugo-book

# generate Markdown from XML:
mkdir -p $OUTDIR
doxybook2 -i xml -o $OUTDIR -c doxybook2-config.json
rm -r website/content/Namespaces ### investigate namespaces with weird names

# generate static HTML from Markdown:
cp hugo-config.toml website/config.toml
cd website
hugo

echo
echo Done. Now do something like \"cp -r `realpath public` $PUBDIR\"
echo
