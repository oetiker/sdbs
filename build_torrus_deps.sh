#!/bin/bash

. `dirname $0`/sdbs.inc


if [ ! -e $PREFIX/bin/perl ]; then
    echo "Perl is required at $PREFIX/bin/perl. Aborting."
    exit 1
fi

if [ ! -e $PREFIX/bin/rrdtool ]; then
    echo "RRDTool is required at $PREFIX/bin/rrdtool. Aborting."
    exit 1
fi


# http://www.oracle.com/
if prepare ${DEPS_ARCHIVE} db-6.0.20.tar.gz ; then
    cd build_unix
    ../dist/configure \
        "--prefix=${PREFIX}"
    make
    make install
    touch $WORKDIR/db-6.0.20.tar.gz.ok
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
