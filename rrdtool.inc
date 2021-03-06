
function mesonbuild (){    
 if prepare $1 $2
 then
   shift
   simpleprogram=$1;shift
   
   CFLAGS="-O3 -fPIC" CPPFLAGS="-I${PREFIX}/include" \
         LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}" \
         ${PREFIX}/bin/meson setup --prefix $PREFIX --libdir=${SDBS_LIBDIR} "$@" _build \
         2>&1 | tee -a ${WORKDIR}/${simpleprogram}.output
   [ ${PIPESTATUS[0]} = 0 ] || exit ${PIPESTATUS[0]}     
   ${PREFIX}/bin/ninja -C _build install 2>&1 | tee -a ${WORKDIR}/${simpleprogram}.output
   [ ${PIPESTATUS[0]} = 0 ] || exit ${PIPESTATUS[0]}     
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
        CFLAGS="-O3 -fPIC" ./configure --prefix=$PREFIX --libdir=${SDBS_LIBDIR}
        CFLAGS="-O3 -fPIC" make install
        touch $WORKDIR/zlib-1.2.11.tar.gz.ok
        remove
    fi

    # http://ftp.gnu.org/pub/gnu/gettext/
    simplebuild ${DEPS_ARCHIVE} gettext-0.20.tar.gz \
        CFLAGS="-O3 -fPIC" --disable-java \
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}" \
        --without-emacs

    # http://sourceware.org/libffi/
    simplebuild ${DEPS_ARCHIVE} libffi-3.3.tar.gz \
        enable_multi_os_directory=no \
        CFLAGS="-O3 -fPIC" \
        CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}"

    # http://xmlsoft.org/sources/
    simplebuild ${DEPS_ARCHIVE} libxml2-2.9.10.tar.gz --without-python

    # glib needs python 3.5 or newer
    ${SDBS_SCRIPTS_DIR}/check_or_build_python3_5.sh
    
    # meson and ninja are needed for a bunch of libraries
    $PREFIX/bin/python3 -m pip install meson
    if [ $? -ne 0 ]; then exit 1; fi
    $PREFIX/bin/python3 -m pip install ninja
    if [ $? -ne 0 ]; then exit 1; fi

    # http://ftp.gnome.org/pub/gnome/sources/glib/
    mesonbuild ${DEPS_ARCHIVE} glib-2.65.0.tar.xz
                
    # gobject-introspection is required by fribidi
    # https://gi.readthedocs.io/en/latest/
    mesonbuild ${DEPS_ARCHIVE} gobject-introspection-1.64.1.tar.xz
    
    # http://sourceforge.net/projects/libpng/files/
    simplebuild ${DEPS_ARCHIVE} libpng-1.6.37.tar.gz \
        CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}"

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
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}"
    
    # fribidi is needed by pango
    # https://github.com/fribidi/fribidi
    mesonbuild ${DEPS_ARCHIVE} fribidi-1.0.10.tar.xz
    
    # http://ftp.gnome.org/pub/GNOME/sources/pango/
    mesonbuild ${DEPS_ARCHIVE} pango-1.45.4.tar.xz

    if [ `id -u` -ne 0 ]; then
        SKIP_SYSTEMD_UNIT='--with-systemdsystemunitdir=no'
    else
        SKIP_SYSTEMD_UNIT=''
    fi

    simplebuild http://oss.oetiker.ch/rrdtool/pub/ \
        rrdtool-${RRDTOOL_VER}.tar.gz \
        --with-perl-options="PREFIX=$PREFIX LIB=$PREFIX/lib/perl5" \
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}" ${SKIP_SYSTEMD_UNIT}
}



# Local Variables:
# mode: shell-script
# indent-tabs-mode: nil
# End:
