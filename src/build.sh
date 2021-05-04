#!/bin/sh
export PATH=$PATH:/home/friedy/Documents/i686-elf-tools-linux/bin
set -e
. ./headers.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
