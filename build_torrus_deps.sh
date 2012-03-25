#!/bin/bash

. `dirname $0`/sdbs.inc

echo "Required prerequisites (recommended to build with SDBS): "
echo "  perl 5.12.4 or 5.14.2"
echo "  rrdtool 1.4.5 or higher"

sleep 3

export PATH=$PREFIX/bin:$PATH

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
