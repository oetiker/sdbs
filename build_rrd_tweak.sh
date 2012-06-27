#!/bin/bash

. `dirname $0`/sdbs.inc


export PATH=$PREFIX/bin:$PATH

export RRDTWEAK_LDFLAGS="-Wl,-rpath=$PREFIX/lib"

for module in \
        RRD::Tweak \
; do
    perlmodule $module
done
