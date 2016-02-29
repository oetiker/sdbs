#!/bin/bash

. `dirname $0`/sdbs.inc

build_python=no

if [ x`which python` = x ]; then
    build_python=yes
else
    if python -V 2>&1 | egrep -q '2.[5-9]'; then
        :
    else
        build_python=yes
    fi
fi

if [ $build_python = yes ]; then
    simplebuild https://www.python.org/ftp/python/2.7.9/ Python-2.7.9.tgz
fi


