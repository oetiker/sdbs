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
        CFLAGS="-O3 -fPIC" ./config --prefix=$PREFIX --openssldir=$PREFIX/ssl --libdir=${SDBS_LIBDIR}
        if CFLAGS="-O3 -fPIC" make install; then
            touch $WORKDIR/openssl-1.1.1g.tar.gz.ok
        fi
    fi
    
    simplebuild https://www.python.org/ftp/python/3.8.5 Python-3.8.5.tgz LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}"

    # dirty hack because python cannot find its libraries in lib64
    if [ ${SDBS_LIBDIR} = ${PREFIX}/lib64 ]; then
        ln -s ${PREFIX}/lib64/python3.8/lib-dynload ${PREFIX}/lib/python3.8/lib-dynload
    fi
fi


