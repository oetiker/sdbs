
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
    if prepare ${DEPS_ARCHIVE} zlib-1.3.1.tar.gz; then
        CFLAGS="-O3 -fPIC" ./configure --prefix=$PREFIX --libdir=${SDBS_LIBDIR}
        CFLAGS="-O3 -fPIC" make install
        touch $WORKDIR/zlib-1.3.1.tar.gz.ok
        remove
    fi

    # http://ftp.gnu.org/pub/gnu/gettext/
    simplebuild ${DEPS_ARCHIVE} gettext-0.22.tar.xz \
        CFLAGS="-O3 -fPIC" --disable-java \
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}" \
        --without-emacs

    # http://sourceware.org/libffi/
    simplebuild ${DEPS_ARCHIVE} libffi-3.4.4.tar.gz \
        enable_multi_os_directory=no \
        CFLAGS="-O3 -fPIC" \
        CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}"

    # http://xmlsoft.org/sources/
    simplebuild ${DEPS_ARCHIVE} libxml2-2.9.12.tar.gz --without-python

    # https://github.com/PCRE2Project/pcre2
    simplebuild ${DEPS_ARCHIVE} pcre2-10.42.tar.bz2

    # glib needs python 3.5 or newer
    ${SDBS_SCRIPTS_DIR}/build_python3_current.sh
    
    # meson and ninja are needed for a bunch of libraries
    $PREFIX/bin/python3 -m pip install meson
    if [ $? -ne 0 ]; then exit 1; fi
    $PREFIX/bin/python3 -m pip install ninja
    if [ $? -ne 0 ]; then exit 1; fi
    # packaging is required by glib
    $PREFIX/bin/python3 -m pip install packaging
    if [ $? -ne 0 ]; then exit 1; fi
    # distutils is required by gobject-introspection; setuptools replaces distutils
    $PREFIX/bin/python3 -m pip install setuptools
    if [ $? -ne 0 ]; then exit 1; fi

    # flex and bison required by gobject-introspection
    # https://github.com/westes/flex
    simplebuild ${DEPS_ARCHIVE} flex-2.6.4.tar.gz

    # http://ftp.gnu.org/gnu/bison/
    simplebuild ${DEPS_ARCHIVE} bison-3.8.tar.xz
     
    if [ ! -e ${WORKDIR}/glib-2.79.1.tar.xz.ok -o ! -e ${WORKDIR}/gobject-introspection-1.79.1.tar.xz.ok ]; then
        # see the explanation for the weird build procedure at https://docs.gtk.org/glib/building.html
        # http://ftp.gnome.org/pub/gnome/sources/glib/
        mesonbuild ${DEPS_ARCHIVE} glib-2.79.1.tar.xz -Dintrospection=disabled
        rm ${WORKDIR}/glib-2.79.1.tar.xz.ok

        # gobject-introspection is required by fribidi
        # https://gi.readthedocs.io/en/latest/
        mesonbuild ${DEPS_ARCHIVE} gobject-introspection-1.79.1.tar.xz
    
        mesonbuild ${DEPS_ARCHIVE} glib-2.79.1.tar.xz -Dintrospection=enabled
    fi

    # http://sourceforge.net/projects/libpng/files/
    simplebuild ${DEPS_ARCHIVE} libpng-1.6.41.tar.xz \
        CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}"

    # http://sourceforge.net/projects/expat/files/
    simplebuild ${DEPS_ARCHIVE} expat-2.5.0.tar.xz

    # http://download.savannah.gnu.org/releases/freetype/
    simplebuild ${DEPS_ARCHIVE} freetype-2.13.2.tar.xz CFLAGS="-O3 -fPIC"

    # gperf required by fontconfig
    # https://www.gnu.org/software/gperf/
    simplebuild ${DEPS_ARCHIVE} gperf-3.1.tar.gz
    
    # http://www.freedesktop.org/software/fontconfig/release/
    simplebuild ${DEPS_ARCHIVE} fontconfig-2.15.0.tar.xz CFLAGS="-O3 -fPIC"

    # https://github.com/harfbuzz/harfbuzz
    mesonbuild ${DEPS_ARCHIVE} harfbuzz-8.3.0.tar.xz -Dbenchmark=disabled
    
    # http://cairographics.org/releases/
    mesonbuild ${DEPS_ARCHIVE} pixman-0.43.0.tar.gz

    # http://cairographics.org/releases/
    mesonbuild ${DEPS_ARCHIVE} cairo-1.18.0.tar.xz
    
    # fribidi is needed by pango
    # https://github.com/fribidi/fribidi
    mesonbuild ${DEPS_ARCHIVE} fribidi-1.0.13.tar.xz
    
    # http://ftp.gnome.org/pub/GNOME/sources/pango/
    mesonbuild ${DEPS_ARCHIVE} pango-1.51.0.tar.xz

    if [ `id -u` -ne 0 ]; then
        SKIP_SYSTEMD_UNIT='--with-systemdsystemunitdir=no'
    else
        SKIP_SYSTEMD_UNIT=''
    fi

    simplebuild https://github.com/oetiker/rrdtool-1.x/releases/download/v${RRDTOOL_VER} \
        rrdtool-${RRDTOOL_VER}.tar.gz \
        --with-perl-options="PREFIX=$PREFIX LIB=$PREFIX/lib/perl5" \
        LDFLAGS="${R}${SDBS_LIBDIR} -L${SDBS_LIBDIR}" ${SKIP_SYSTEMD_UNIT}

    echo Running dirty hack to go around meson stripping rpath
    for f in ${SDBS_LIBDIR}/*.so; do $PREFIX/bin/patchelf --set-rpath ${SDBS_LIBDIR} $f; done
}



# Local Variables:
# mode: shell-script
# indent-tabs-mode: nil
# End:
