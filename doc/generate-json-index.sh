#!/bin/bash

COUNTER=1
echo "["
for f in `find doxybook-out/classes/ -name 'class*.md'|sort`; do
    NAME=`basename $f|sed 's/^class//'|sed 's/\.md$//'`
    echo "    {\"id\": $COUNTER, \"text\": \"$NAME\"},"
    COUNTER=$((COUNTER + 1))
done
echo "]"
