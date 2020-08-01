
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
    
    # glib needs meson and ninja
    $PREFIX/bin/python3 -m pip install meson
    if [ $? -ne 0 ]; then exit 1; fi
    $PREFIX/bin/python3 -m pip install ninja
    if [ $? -ne 0 ]; then exit 1; fi

    # http://ftp.gnome.org/pub/gnome/sources/glib/
    if prepare http://ftp.gnome.org/pub/gnome/sources/glib/2.65/ glib-2.65.0.tar.xz; then
        if CFLAGS="-O3 -fPIC" CPPFLAGS="-I${PREFIX}/include" \
                 LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib" ${PREFIX}/bin/meson _build; then
            if ${PREFIX}/bin/ninja -C _build install; then
                touch $WORKDIR/glib-2.65.0.tar.xz.ok
            else
                exit 1
            fi
        else
            exit 1
        fi
    fi
                
        
    
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
    simplebuild ${DEPS_ARCHIVE} harfbuzz-2.6.7.tar.xz CFLAGS="-O3 -fPIC"
    
    # http://cairographics.org/releases/
    simplebuild ${DEPS_ARCHIVE} pixman-0.40.0.tar.gz \
        CFLAGS="-O3 -fPIC" \
        --disable-static-testprogs \
        --disable-gtk        

    # http://cairographics.org/releases/
    simplebuild ${DEPS_ARCHIVE} cairo-1.16.0.tar.xz \
        --enable-xlib=no --enable-xlib-render=no --enable-win32=no  \
        CFLAGS="-O3 -fPIC" CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib"

    # http://ftp.gnome.org/pub/GNOME/sources/pango/
    simplebuild ${DEPS_ARCHIVE} pango-1.45.4.tar.xz \
        CFLAGS="-O3 -fPIC"

    simplebuild http://oss.oetiker.ch/rrdtool/pub/ \
        rrdtool-${RRDTOOL_VER}.tar.gz \
        --with-perl-options="PREFIX=$PREFIX LIB=$PREFIX/lib/perl5"
}



# Local Variables:
# mode: shell-script
# indent-tabs-mode: nil
# End:
