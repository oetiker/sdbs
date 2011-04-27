#!/bin/bash

. `dirname $0`/sdbs.inc

if [ -d /usr/pack/postgresql-8.4.3-za ]; then
  export POSTGRES_LIB=/usr/pack/postgresql-8.4.3-za/amd64-linux-ubuntu8.04/lib \
  export POSTGRES_INCLUDE=/usr/pack/postgresql-8.4.3-za/amd64-linux-ubuntu8.04/include
fi

for module in \
    Mojolicious \
    MojoX::Dispatcher::Qooxdoo::Jsonrpc \
    Config::Grammar \
    DBI \
    DBD::Pg \
    JSON::XS \
    Mojo::JSON::Any \
    Try::Tiny \
; do
    perlmodule $module
done

    
