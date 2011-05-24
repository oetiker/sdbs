#!/bin/bash

. `dirname $0`/sdbs.inc

simplebuild http://www.python.org/ftp/python/2.7.1 Python-2.7.1.tgz

for module in \
    Mojolicious \
    MojoX::Dispatcher::Qooxdoo::Jsonrpc \
    Config::Grammar \
    SIAM \
    DBD::SQLite \
    JSON::XS \
    Mojo::JSON::Any \
; do
    perlmodule $module
done

        
