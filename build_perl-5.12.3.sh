#!/bin/bash
. `dirname $0`/sdbs.inc

multiarch_dir=${DEB_HOST_MULTIARCH:-$(dpkg-architecture -qDEB_HOST_MULTIARCH 2>/dev/null)}

if [ -n "$multiarch_dir" ]; then
   extra_path="-Dplibpth=/lib/$multiarch_dir /usr/lib/$multiarch_dir"
else
   extra_path=""
fi

if prepare http://search.cpan.org/CPAN/authors/id/R/RJ/RJBS perl-5.12.3.tar.gz; then
   make clean || true
   ./Configure -de \
        -Ui_db \
        -Dprivlib=$PREFIX/lib/perl \
        -Dsitelib=$PREFIX/lib/perl5 \
        -Dprefix=$PREFIX \
        -Dperlpath="$PREFIX/bin/perl" \
        -Dstartperl="#!$PREFIX/bin/perl" \
        -Dusethreads "$extra_path" \
        -de
   make
   make install
   touch $WORKDIR/perl-5.12.3.tar.gz.ok
fi



