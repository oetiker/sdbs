## other decision structure
set -o errexit
# don't try to remember where things are
set +o hashall
## do not tolerate unset variables
set -u

## check if wget is available
if [ x`which wget` = x ]; then
 echo "cannot find wget in PATH. Aborting"
 exit 1
fi 

bindir=`dirname "$0"`
SDBS_SCRIPTS_DIR=`cd $bindir;pwd`

PREFIX=${1:-${PREFIX:-""}}

if [ ${PREFIX:-""} = "" ]; then
     PREFIX=`cd ${SDBS_SCRIPTS_DIR}/..;pwd`/thirdparty
fi
export PREFIX
echo "Building in $PREFIX"

EPREFIX=${EPREFIX:-$PREFIX}
WORKDIR=${WORKDIR:-$PREFIX/src}

[ -d $PREFIX/bin ] || mkdir -p $PREFIX/bin
[ -d $WORKDIR ] || mkdir -p $WORKDIR

if [ ${SDBS_LIBDIR:-""} = "" ]; then
    if [ `getconf LONG_BIT` -eq 64 ]; then
        SDBS_LIBDIR=$EPREFIX/lib64
    else
        SDBS_LIBDIR=$EPREFIX/lib
    fi
fi


    
# default
KEEP=no

# Our own repository with dependencies
DEPS_ARCHIVE=${DEPS_ARCHIVE:-http://sdbs.oetiker.ch/pub/}

# make sure we find anything we preinstall
export PATH=$EPREFIX/bin:$PATH
# tell pkg-config where to look for *.pc files
export PKG_CONFIG_PATH=${SDBS_LIBDIR}/pkgconfig
# just to make sure those who are not compiled propperly still work
export LD_LIBRARY_PATH=${SDBS_LIBDIR}
# find our own perl modules
export PERL5LIB=$EPREFIX/lib/perl5
if [ ${PERL:-x} != x ]; then
   PATH=`dirname $PERL`:$PATH
fi
export PERL=perl
export PERL_CPANM_HOME="$PREFIX"
export PERL_CPANM_OPT="--notest --local-lib $PREFIX"

if [ `uname` = SunOS ]; then
  R="-R"
else
  R="-Wl,-rpath -Wl,"
fi

function prepare () {
 cd $WORKDIR
 if [ ! -f $2.ok ]
 then 
   echo "**** doing $2 ****"
   [ -f $2 ] || wget --no-check-certificate --tries=0 --random-wait --passive-ftp $1/$2
   unset SRCDIR
   [ -f $2.srcdir ] && SRCDIR=`cat $2.srcdir`
   case $2 in
     *.tar.bz2)
         SRCDIR=${SRCDIR:-$WORKDIR/`basename $2 .tar.bz2`}
         [ -d $SRCDIR ] || bunzip2 -c $2 | tar xf -;;
     *.tar.gz)
         SRCDIR=${SRCDIR:-$WORKDIR/`basename $2 .tar.gz`}
         [ -d $SRCDIR ] || gunzip -c $2 | tar xf -;;
     *.tar.xz)
         SRCDIR=${SRCDIR:-$WORKDIR/`basename $2 .tar.xz`}
         [ -d $SRCDIR ] || xz --decompress --stdout $2 | tar xf -;;
     *.tgz)
         SRCDIR=${SRCDIR:-$WORKDIR/`basename $2 .tgz`}
         [ -d $SRCDIR ] || gunzip -c $2 | tar xf -;;
     *.tar.Z)
         SRCDIR=${SRCDIR:-$WORKDIR/`basename $2 .tar.Z`}
         [ -d $SRCDIR ] || gunzip -c $2 | tar xf -;;
     *.tar)
         SRCDIR=${SRCDIR:-$WORKDIR/`basename $2 .tar`}
         [ -d $SRCDIR ] || tar xf $2;;
     *.zip)
         SRCDIR=${SRCDIR:-$WORKDIR/`basename $2 .zip`}
	 [ -d $SRCDIR ] || unzip -a $2 || exit 1;;

     *)  echo "Don't know how to unpack $2"
   esac
   if [ ! -d $SRCDIR ]; then
   	SRCDIR=`ls -F1tc $WORKDIR | grep / | head -1 | sed 's/\/$//'`
	echo $SRCDIR >$2.srcdir
   fi
   cd $SRCDIR
 else
   echo "**** skipping $2 ****"
   cd $WORKDIR
   return 1
 fi
}

function simplebuild (){    
 if prepare $1 $2
 then
   shift
   simpleprogram=$1;shift   
   if [ ! -f configure ]; then
        confdir=`ls -1f source/configure src/configure */configure| head -1`
        echo "found $confdir"
        upone=".."
        if [ -f "$confdir" ]; then
                cd `dirname $confdir`
        else
           confdir=`ls */*/configure | head -1`
           upone="../.."
           if [ -f "$confdir" ]; then
               cd `dirname $confdir`  
           else
                echo "No configure script found in $simpleprogram"
                exit 1
           fi
        fi
   else
      upone="."
   fi
   # make sure our libraries come first because only this guarantees propper operation of xrender
   answer=N
   if test -f config.log -a -f Makefile ; then
     printf "Reconfigure? (y/N) "
     read answer
   fi
   if test !  -f config.log  -o ! -f Makefile -o   "$answer" = y  -o  "$answer" = Y ; then
     rm -f config.cache 
     ./configure --exec-prefix=$EPREFIX --prefix=$PREFIX "$@" --libdir=${SDBS_LIBDIR} \
                 2>&1 | tee -a $upone/../${simpleprogram}.output
     [ ${PIPESTATUS[0]} = 0 ] || exit ${PIPESTATUS[0]}     
     if [ -f $WORKDIR/patch.post ]; then
        gpatch -p0 -l <$WORKDIR/patch.post 
     fi
   fi
   make                                                       2>&1 | tee -a $upone/../${simpleprogram}.output
   [ ${PIPESTATUS[0]} = 0 ] || exit ${PIPESTATUS[0]}     
   make install                                               2>&1 | tee -a $upone/../${simpleprogram}.output
   [ ${PIPESTATUS[0]} = 0 ] || exit ${PIPESTATUS[0]}     
   cd $upone                                                   
   remove 
   touch ${simpleprogram}.ok                                   
 fi
 [ -f $WORKDIR/patch.post ] && rm $WORKDIR/patch.post
 cd $WORKDIR
}


function remove () {
    DIR=`pwd`    
    case $DIR in
        $WORKDIR/*)
                cd ..
                if [ x$KEEP = xyes ]; then
                        echo Keeping $DIR
                else
                        rm -rf $DIR
                fi
                ;;
        *)
                echo OOPS I wont remove $DIR because it is not in $WORKDIR
                exit 1
                ;;
    esac
}

function perlmodule (){
    path=$1;shift
    pack=${1:-""}
    if [ ! -e $PREFIX/bin/cpanm ]; then
       echo "Cannot find $PREFIX/bin/perl or $PREFIX/bin/cpanm. Aborting"
       exit 1
    fi
    cpanm "$path$pack"
    [ "$KEEP" = "YES" ] || rm -rf $PREFIX/work $PREFIX/latest-build
}


# vim: ft=sh

# Local Variables:
# mode: shell-script
# indent-tabs-mode: nil
# End:
