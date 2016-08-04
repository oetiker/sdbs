#!/bin/bash

export PERL=/usr/bin/perl

. `dirname $0`/sdbs.inc
if prepare https://download.bestpractical.com/pub/rt/release/ rt-4.4.1.tar.gz; then
    ./configure  --with-db-type=SQLite --enable-gd --enable-graphviz --prefix=$PREFIX --with-my-user-group 
    mkdir -p $PREFIX/bin
    wget --no-check-certificate -O $PREFIX/bin/cpanm cpanmin.us && chmod 755 $PREFIX/bin/cpanm

    for module in `make testdeps | grep MISSING | awk '{print $1}'`; do
       perlmodule $module
    done
    make testdeps
    make install
    bash
fi



        
