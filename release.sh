#!/bin/sh
# Create source tarball from Git tag

APP=billboard
VERSION=$(LC_ALL=C dpkg-parsechangelog | grep Version: | cut -d' ' -f2-)
OUTFILE=${APP}-${VERSION}.tar.gz

git archive --format=tar --prefix=$APP-$VERSION/ $VERSION | gzip -9 >$OUTFILE
