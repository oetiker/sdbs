#!/bin/bash

export PERL=/usr/bin/perl
id rt4 >/dev/null 2>&1 || ( echo rt4 user is missing; exit 1 )

. `dirname $0`/sdbs.inc
if prepare https://download.bestpractical.com/pub/rt/release/ rt-4.4.1.tar.gz; then
    ./configure  --with-db-type=Pg --enable-gd --enable-graphviz --prefix=$PREFIX \
        --with-web-user=rt4 \
        --with-web-group=rt4
    mkdir -p $PREFIX/bin
    wget --no-check-certificate -O $PREFIX/bin/cpanm cpanmin.us && chmod 755 $PREFIX/bin/cpanm
    for module in `make testdeps | grep MISSING | awk '{print $1}' | grep -v SOME`; do
       perlmodule $module
    done
    perlmodule Starman
    make testdeps
    make install
    bash
fi



        
