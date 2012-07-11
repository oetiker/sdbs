#!/bin/bash

. `dirname $0`/sdbs.inc

if [ x`which pkg-config` = x ]; then
  echo "Cannot find pkg-config in PATH. Aborting."
  exit 1
fi

# make our own zlib.
# Other packages do not use pkg-config  for finding zlib, so we 
# have to set LDFLAGS and CPPFLAGS explicitly
if prepare http://switch.dl.sourceforge.net/project/libpng/zlib/1.2.7/ \
 zlib-1.2.7.tar.gz; then
   CFLAGS="-O3 -fPIC" ./configure --prefix=$PREFIX
   CFLAGS="-O3 -fPIC" make install
   touch $WORKDIR/zlib-1.2.5.tar.gz.ok
fi

simplebuild http://ftp.gnu.org/pub/gnu/gettext/ gettext-0.18.1.1.tar.gz \
  CFLAGS="-O3 -fPIC" --disable-java

simplebuild http://xmlsoft.org/sources/ libxml2-2.7.8.tar.gz --without-python

simplebuild \
http://switch.dl.sourceforge.net/project/libpng/libpng14/older-releases/1.4.11/ \
libpng-1.4.11.tar.gz CPPFLAGS="-I${PREFIX}/include" LDFLAGS="-L${PREFIX}/lib"

simplebuild http://switch.dl.sourceforge.net/project/expat/expat/2.1.0/ \
  expat-2.1.0.tar.gz

simplebuild http://download.savannah.gnu.org/releases/freetype/ \
  freetype-2.4.10.tar.gz CFLAGS="-O3 -fPIC"

simplebuild http://www.freedesktop.org/software/fontconfig/release/ \
  fontconfig-2.8.0.tar.gz CFLAGS="-O3 -fPIC"

simplebuild http://cairographics.org/releases/ pixman-0.26.2.tar.gz \
  CFLAGS="-O3 -fPIC" \
  --disable-static-testprogs \
  --disable-gtk        

simplebuild http://cairographics.org/releases/ cairo-1.12.2.tar.gz \
  --enable-xlib=no --enable-xlib-render=no --enable-win32=no  \
  CFLAGS="-O3 -fPIC" CPPFLAGS="-I${PREFIX}/include" LDFLAGS="-L${PREFIX}/lib"

simplebuild http://ftp.gnome.org/pub/gnome/sources/glib/2.33/ \
  glib-2.33.3.tar.gz CFLAGS="-O3 -fPIC" \
  CPPFLAGS="-I${PREFIX}/include" LDFLAGS="-L${PREFIX}/lib"

simplebuild http://ftp.gnome.org/pub/GNOME/sources/pango/1.30/ \
  pango-1.30.1.tar.gz --without-x CFLAGS="-O3 -fPIC"

simplebuild http://oss.oetiker.ch/rrdtool/pub/ rrdtool-1.4.7.tar.gz \
  --with-perl-options="PREFIX=$PREFIX LIB=$PREFIX/lib/perl5"


