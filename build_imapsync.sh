#!/bin/bash

export PERL=/usr/bin/perl

. `dirname $0`/sdbs.inc

wget --no-check-certificate -O $PREFIX/bin/cpanm cpanmin.us && chmod 755 $PREFIX/bin/cpanm


KEEP=yes
#perlmodule Starman
#perlmodule HTML::Gumbo

perlmodule  Digest::HMAC_MD5
perlmodule File::Temp
perlmodule Term::ReadKey
perlmodule Parse::RecDescent
perlmodule Test::Harness
perlmodule Module::Build
perlmodule Date::Manip
perlmodule Net::LibIDN
perlmodule Net::SSLeay
perlmodule IO::Socket::SSL
perlmodule IO::Tee
perlmodule Mail::IMAPClient
perlmodule Unicode::String
perlmodule Readonly
perlmodule Sys::MemInfo
perlmodule Regexp::Common

if prepare http://imapsync.lamiral.info/dist imapsync-1.882.tgz
   then
   echo done
fi
