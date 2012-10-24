#!/bin/bash
. `dirname $0`/sdbs.inc

# unset PERL settings in sdbs for building perl
export PERL5LIB=
export PERL=
export PERL_CPANM_HOME=
export PERL_CPANM_OPT=

if prepare http://www.cpan.org/src/5.0 perl-5.16.1.tar.gz; then
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
   touch $WORKDIR/perl-5.16.1.tar.gz.ok
fi

# EOF
