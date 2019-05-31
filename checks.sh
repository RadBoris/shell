#!/bin/bash

if [ $# != 2 ] ; then
    echo "usage $0 PATH_TO_BINARY TARGET_FOLDER"
    exit 1
fi

PATH_TO_BINARY="$1"
TARGET_FOLDER="$2"


if [ ! -f "$PATH_TO_BINARY" ] ; then
    echo "The file '$PATH_TO_BINARY' was not found. Aborting!"
    exit 1
fi

cp --parents -v "$PATH_TO_BINARY" "$TARGET_FOLDER"

for lib in `ldd "$PATH_TO_BINARY" | cut -d'>' -f2 | awk '{print $1}'` ; do
   if [ -f "$lib" ] ; then
        cp -v --parents "$lib" "$TARGET_FOLDER"
   fi  
done

if [ ! -d "$TARGET_FOLDER/lib64" ] ; then
    mkdir -v "$TARGET_FOLDER/lib64"
fi


