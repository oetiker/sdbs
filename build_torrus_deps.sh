#!/bin/bash

. `dirname $0`/sdbs.inc

echo "you may ./build_perl-5.12.4.sh to get a recent version of perl up"
echo "5.14.1 does not work because FCGI-0.69 does not build"
echo "until https://rt.cpan.org/Public/Bug/Display.html?id=68380 if fixed we are stuck"

sleep 3

export PATH=$PREFIX/bin:$PATH

simplebuild ftp://xmlsoft.org/libxml2/ libxml2-2.7.8.tar.gz --without-python

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
        MSTROUT/FCGI-0.69.tar.gz \
; do
    perlmodule $module
done
