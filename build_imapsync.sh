#!/bin/bash

export PERL=/usr/bin/perl

. `dirname $0`/sdbs.inc

wget --no-check-certificate -O $PREFIX/bin/cpanm cpanmin.us && chmod 755 $PREFIX/bin/cpanm


KEEP=yes
perlmodule Digest::HMAC_MD5
perlmodule Digest::HMAC_SHA1
perlmodule File::Temp
perlmodule File::Tail
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
perlmodule Authen::NTLM
perlmodule Crypt::OpenSSL::RSA
perlmodule Data::Uniqid
perlmodule JSON::WebToken
perlmodule Test::MockObject
perlmodule Encode::IMAPUTF7

if prepare http://imapsync.lamiral.info/dist imapsync-1.977.tgz
   then
   cp imapsync $PREFIX/bin
fi
