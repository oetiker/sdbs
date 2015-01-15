#!/bin/bash

. `dirname $0`/sdbs.inc

# qooxdoo needs python 2.5 or newer
${SDBS_SCRIPTS_DIR}/check_or_build_python2_5.sh


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

        
