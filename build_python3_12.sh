#!/bin/bash

. `dirname $0`/sdbs.inc

if prepare https://www.openssl.org/source openssl-1.1.1w.tar.gz; then
    CFLAGS="-O3 -fPIC" ./config --prefix=$PREFIX --openssldir=$PREFIX/ssl --libdir=${SDBS_LIBDIR}
    if CFLAGS="-O3 -fPIC" make install; then
        touch $WORKDIR/openssl-1.1.1w.tar.gz.ok
    fi
fi
    
simplebuild https://www.python.org/ftp/python/3.12.1 Python-3.12.1.tgz LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}"

# dirty hack because python cannot find its libraries in lib64
if [ ${SDBS_LIBDIR} = ${PREFIX}/lib64 ] && [ ! -e ${PREFIX}/lib/python3.12/lib-dynload ]; then
    ln -s ${PREFIX}/lib64/python3.12/lib-dynload ${PREFIX}/lib/python3.12/lib-dynload
fi



