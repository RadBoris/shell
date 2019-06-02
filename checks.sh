#!/bin/bash

croot="new-root"
cur_bash="/bin/bash"

if [ ! -d "$PWD/new-root" ] ; then
    echo "Creating target dir..."
    mkdir $PWD/new-root


cp --parents -v "$cur_bash" "$PWD/new-root"

for lib in `ldd "$cur_bash" | cut -d'>' -f2 | awk '{print $1}'` ; do
   if [ -f "$lib" ] ; then
        cp -v --parents "$lib" "$PWD/new-root"
   fi
done

if [ ! -d "$PWD/new-root/lib64" ] ; then
    mkdir -v "$PWD/new-root/lib64"
fi

echo "Done copying bash dependencies"

chroot $PWD/new-root /bin/bash

echo "In new root"

fi

chroot $PWD/new-root /bin/bash

echo "In new root. Jail dir existed"
