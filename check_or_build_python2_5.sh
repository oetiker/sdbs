#!/bin/bash

. `dirname $0`/sdbs.inc

if python -V 2>&1 | egrep -q '2.[5-9]'; then
:
else
   simplebuild https://www.python.org/ftp/python/2.7.9/ \
    Python-2.7.9.tgz
fi

