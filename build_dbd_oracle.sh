#!/bin/bash

. `dirname $0`/sdbs.inc

if [ x"$ORACLE_HOME" = x ]; then
   echo ORACLE_HOME is not set
   exit 1
fi

for libdir in $ORACLE_HOME/lib $ORACLE_HOME; do
  if [ -e $libdir/libclntsh.so ]; then
    break
  fi
done


if [ ! -e $libdir/libclntsh.so ]; then
   echo "ORACLE_HOME does not point to Oracle instant client"
   exit 1;
fi
export LD_LIBRARY_PATH=$libdir

perlmodule DBI
perlmodule DBD::Oracle
