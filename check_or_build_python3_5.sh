#!/bin/bash

. `dirname $0`/sdbs.inc

build_python=no

if [ x`which python3` = x ]; then
    build_python=yes
else
    if python3 -V 2>&1 | egrep -q '3.[5-9]'; then
        :
    else
        build_python=yes
    fi
fi

build_python=yes

if [ $build_python = yes ]; then
    if prepare https://www.openssl.org/source openssl-1.1.1g.tar.gz; then
        CFLAGS="-O3 -fPIC" ./config --prefix=$PREFIX --openssldir=$PREFIX/ssl
        if CFLAGS="-O3 -fPIC" make install; then
            touch $WORKDIR/openssl-1.1.1g.tar.gz.ok
        fi
    fi
    
    simplebuild https://www.python.org/ftp/python/3.8.5 Python-3.8.5.tgz LDFLAGS="-L$PREFIX/lib64 -L$PREFIX/lib"
fi


