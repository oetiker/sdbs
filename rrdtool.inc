
function mesonbuild (){    
 if prepare $1 $2
 then
   shift
   simpleprogram=$1;shift
   
   CFLAGS="-O3 -fPIC" CPPFLAGS="-I${PREFIX}/include" \
         LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib ${R}${PREFIX}/lib64 -L${PREFIX}/lib64" \
         ${PREFIX}/bin/meson setup --prefix $PREFIX "$@" _build \
         2>&1 | tee -a ${WORKDIR}/${simpleprogram}.output
   if [ $? -ne 0 ]; then exit 1; fi
   ${PREFIX}/bin/ninja -C _build install 2>&1 | tee -a ${WORKDIR}/${simpleprogram}.output
   if [ $? -ne 0 ]; then exit 1; fi
   remove 
   touch ${WORKDIR}/${simpleprogram}.ok
 fi
}


function build_rrdtool_1_4 (){    
    RRDTOOL_VER=$1

    if [ x`which pkg-config` = x ]; then
        echo "Cannot find pkg-config in PATH. Aborting."
        exit 1
    fi
    
    if [ x`which xz` = x ]; then
        echo "Cannot find xz in PATH. Aborting."
        exit 1
    fi

    if [ ! -e $PREFIX/bin/perl ]; then
        echo "Perl is required at $PREFIX/bin/perl. Aborting."
        exit 1
    fi
    
    # Other packages do not use pkg-config  for finding zlib, so we 
    # have to set LDFLAGS and CPPFLAGS explicitly
    # https://zlib.net/
    if prepare ${DEPS_ARCHIVE} zlib-1.2.11.tar.gz; then
        CFLAGS="-O3 -fPIC" ./configure --prefix=$PREFIX
        CFLAGS="-O3 -fPIC" make install
        touch $WORKDIR/zlib-1.2.11.tar.gz.ok
    fi

    # http://ftp.gnu.org/pub/gnu/gettext/
    simplebuild ${DEPS_ARCHIVE} gettext-0.20.tar.gz \
        CFLAGS="-O3 -fPIC" --disable-java \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib" \
        --without-emacs

    # http://sourceware.org/libffi/
    simplebuild ${DEPS_ARCHIVE} libffi-3.3.tar.gz \
        CFLAGS="-O3 -fPIC" \
        CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib"

    # http://xmlsoft.org/sources/
    simplebuild ${DEPS_ARCHIVE} libxml2-2.9.10.tar.gz --without-python

    # glib needs python 3.5 or newer
    ${SDBS_SCRIPTS_DIR}/check_or_build_python3_5.sh
    
    # meson and ninja are needed for a bunch of libraries
    $PREFIX/bin/python3 -m pip install meson
    if [ $? -ne 0 ]; then exit 1; fi
    $PREFIX/bin/python3 -m pip install ninja
    if [ $? -ne 0 ]; then exit 1; fi

    # gobject-introspection is required by fribidi
    # https://gi.readthedocs.io/en/latest/
    mesonbuild ${DEPS_ARCHIVE} gobject-introspection-1.64.1.tar.xz

    # http://ftp.gnome.org/pub/gnome/sources/glib/
    mesonbuild ${DEPS_ARCHIVE} glib-2.65.0.tar.xz
                
        
    
    # http://sourceforge.net/projects/libpng/files/
    simplebuild ${DEPS_ARCHIVE} libpng-1.6.37.tar.gz \
        CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib"

    # http://sourceforge.net/projects/expat/files/
    simplebuild ${DEPS_ARCHIVE} expat-2.2.9.tar.gz

    # http://download.savannah.gnu.org/releases/freetype/
    simplebuild ${DEPS_ARCHIVE} freetype-2.10.2.tar.gz CFLAGS="-O3 -fPIC"

    # gperf required by fontconfig
    # https://www.gnu.org/software/gperf/
    simplebuild ${DEPS_ARCHIVE} gperf-3.1.tar.gz
    
    # http://www.freedesktop.org/software/fontconfig/release/
    simplebuild ${DEPS_ARCHIVE} fontconfig-2.13.92.tar.gz CFLAGS="-O3 -fPIC"

    # https://www.freedesktop.org/software/harfbuzz/release/
    mesonbuild ${DEPS_ARCHIVE} harfbuzz-2.7.0.tar.gz -Dbenchmark=disabled
    
    # http://cairographics.org/releases/
    simplebuild ${DEPS_ARCHIVE} pixman-0.40.0.tar.gz \
        CFLAGS="-O3 -fPIC" \
        --disable-static-testprogs \
        --disable-gtk        

    # http://cairographics.org/releases/
    simplebuild ${DEPS_ARCHIVE} cairo-1.16.0.tar.xz \
        --enable-xlib=no --enable-xlib-render=no --enable-win32=no  \
        CFLAGS="-O3 -fPIC" CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib ${R}${PREFIX}/lib64 -L${PREFIX}/lib64"
    
    # fribidi is needed by pango
    # https://github.com/fribidi/fribidi
    mesonbuild ${DEPS_ARCHIVE} fribidi-1.0.10.tar.xz
    
    # http://ftp.gnome.org/pub/GNOME/sources/pango/
    mesonbuild pango-1.45.4.tar.xz

    simplebuild http://oss.oetiker.ch/rrdtool/pub/ \
        rrdtool-${RRDTOOL_VER}.tar.gz \
        --with-perl-options="PREFIX=$PREFIX LIB=$PREFIX/lib/perl5" \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib ${R}${PREFIX}/lib64 -L${PREFIX}/lib64"
}



# Local Variables:
# mode: shell-script
# indent-tabs-mode: nil
# End:
