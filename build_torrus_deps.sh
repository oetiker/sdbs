#!/bin/bash

. `dirname $0`/sdbs.inc

echo "You may want to build Perl before running this"
echo "Packages required: zlib-devel and libxml2-devel (RHEL)"
echo "libz-dev libxml2-dev (Debian)"

sleep 3

export PATH=$PREFIX/bin:$PATH

# this needs some more work. 
# It still fails without zlib-devel and libxml2-devel
# simplebuild http://xmlsoft.org/sources/ libxml2-2.7.8.tar.gz --without-python

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
        Date::Parse \
        Apache::Session \
        JSON::XS \
        JSON \
        CGI::Fast \
        FCGI \
; do
    perlmodule $module
done
