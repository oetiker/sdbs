#!/bin/bash

export PERL=/usr/bin/perl

. `dirname $0`/sdbs.inc

mkdir -p src
cd src
git config --global pull.rebase true
git clone https://freeswitch.org/stash/scm/fs/freeswitch.git freeswitch
cd freeswitch
./bootstrap.sh -j
perl -i -p -0777 -e 's/#event_handlers\/mod_snmp/event_handlers\/mod_snmp/gs' modules.conf
perl -i -p -0777 -e 's/#languages\/mod_perl/languages\/mod_perl/gs' modules.conf
perl -i -p -0777 -e 's/#applications\/mod_esl/applications\/mod_esl/gs' modules.conf
./configure --enable-64 --enable-zrtp --enable-system-lua
make install
make cd-sounds-install cd-moh-install





        
