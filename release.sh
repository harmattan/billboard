#!/bin/sh
# Create source tarball from current source tree

APP=billboard
VERSION=2.0.1
OUTFILE=${APP}-${VERSION}.tar.gz

tar -czvf $OUTFILE \
    --xform "s#^#${APP}-${VERSION}/#g" \
    --exclude=.git \
    --exclude=.gitmodules \
    --exclude=release.sh \
    --exclude=$OUTFILE \
    *
