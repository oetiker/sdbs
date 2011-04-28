#!/bin/bash

. `dirname $0`/sdbs.inc

simplebuild ftp://xmlsoft.org/libxml2/ libxml2-2.7.8.tar.gz --without-python

simplebuild ftp://ftp.simplesystems.org/pub/libpng/png/src/ libpng-1.5.2.tar.gz

simplebuild http://download.savannah.gnu.org/releases/freetype/ freetype-2.4.4.tar.gz \
     CFLAGS="-O3 -fPIC"

simplebuild http://www.freedesktop.org/software/fontconfig/release/ fontconfig-2.8.0.tar.gz \
     CFLAGS="-O3 -fPIC"

simplebuild http://cairographics.org/releases/ pixman-0.20.2.tar.gz CFLAGS="-O3 -fPIC"

simplebuild http://cairographics.org/releases/ cairo-1.10.2.tar.gz \
        --enable-xlib=no \
        --enable-xlib-render=no \
        --enable-win32=no \
        CFLAGS="-O3 -fPIC"

simplebuild http://ftp.gnome.org/pub/gnome/sources/glib/2.28/glib-2.28.6.tar.gz CFLAGS="-O3 -fPIC"

simplebuild http://ftp.gnome.org/pub/GNOME/sources/pango/1.28/pango-1.28.4.tar.gz --without-x CFLAGS="-O3 -fPIC"




if prepare http://download.oracle.com/berkeley-db/ db-4.8.30.tar.gz ; then
    cd build_unix
    ../dist/configure \
        "--prefix=${PREFIX}"
    make
    make install
    touch $WORKDIR/db-4.8.30.tar.gz.ok
fi

export BERKELEYDB_INCLUDE=$PREFIX/include
export BERKELEYDB_LIB=$PREFIX/lib

for module in \
        XML::LibXML \
        BerkeleyDB \
        Template \
        Proc::Daemon \
        Crypt::DES \
        Crypt::Rijndael \
        Digest::HMAC \
        Digest::SHA1 \
        Net::SNMP \
        URI::Escape \
        Date::Parse\
; do
    perlmodule $module
done
