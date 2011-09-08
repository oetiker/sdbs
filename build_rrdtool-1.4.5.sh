#!/bin/bash

. `dirname $0`/sdbs.inc

if prepare http://zlib.net/ zlib-1.2.5.tar.gz; then
   CFLAGS="-O3 -fPIC" ./configure --prefix=$PREFIX
   CFLAGS="-O3 -fPIC" make install
   touch $WORKDIR/zlib-1.2.5.tar.gz.ok
fi

simplebuild http://ftp.gnu.org/pub/gnu/gettext/ gettext-0.18.1.1.tar.gz CFLAGS="-O3 -fPIC" \
        --disable-java

simplebuild ftp://xmlsoft.org/libxml2/ libxml2-2.7.8.tar.gz --without-python

simplebuild ftp://ftp.simplesystems.org/pub/libpng/png/src/ libpng-1.4.8.tar.gz

simplebuild http://download.savannah.gnu.org/releases/freetype/ freetype-2.4.6.tar.gz \
     CFLAGS="-O3 -fPIC"

simplebuild http://www.freedesktop.org/software/fontconfig/release/ fontconfig-2.8.0.tar.gz \
     CFLAGS="-O3 -fPIC"

simplebuild http://cairographics.org/releases/ pixman-0.22.2.tar.gz CFLAGS="-O3 -fPIC" \
        --disable-static-testprogs \
        --disable-gtk        

simplebuild http://cairographics.org/releases/ cairo-1.10.2.tar.gz \
     --enable-xlib=no --enable-xlib-render=no --enable-win32=no  CFLAGS="-O3 -fPIC"

simplebuild http://ftp.gnome.org/pub/gnome/sources/glib/2.28/ glib-2.28.7.tar.gz CFLAGS="-O3 -fPIC"

simplebuild http://ftp.gnome.org/pub/GNOME/sources/pango/1.28/ pango-1.28.4.tar.gz --without-x CFLAGS="-O3 -fPIC"

simplebuild http://oss.oetiker.ch/rrdtool/pub/ rrdtool-1.4.5.tar.gz --with-perl-options="PREFIX=$PREFIX LIB=$PREFIX/lib/perl5"


