#!/bin/bash

. `dirname $0`/sdbs.inc


for module in \
    Mojolicious \
    Mojo::Server::FastCGI \
    MojoX::Dispatcher::Qooxdoo::Jsonrpc \
    Config::Grammar \
    SIAM \
    DBD::SQLite \
    Excel::Writer::XLSX  \
    Spreadsheet::WriteExcel \
; do
    perlmodule $module
done

        
