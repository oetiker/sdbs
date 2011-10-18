#!/bin/bash
. `dirname $0`/sdbs.inc

if prepare http://search.cpan.org/CPAN/authors/id/F/FL/FLORA perl-5.14.2.tar.gz; then
   make clean || true
   ./Configure -de \
        -Ui_db \
        -Dprivlib=$PREFIX/lib/perl \
        -Dsitelib=$PREFIX/lib/perl5 \
        -Dprefix=$PREFIX \
        -Dperlpath="$PREFIX/bin/perl" \
        -Dstartperl="#!$PREFIX/bin/perl" \
        -Dusethreads -de
   make
   make install
   touch $WORKDIR/perl-5.14.2.tar.gz.ok
fi



