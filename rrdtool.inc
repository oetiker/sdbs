
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
    # http://sourceforge.net/projects/libpng/files/
    if prepare ${DEPS_ARCHIVE} zlib-1.2.8.tar.gz; then
        CFLAGS="-O3 -fPIC" ./configure --prefix=$PREFIX
        CFLAGS="-O3 -fPIC" make install
        touch $WORKDIR/zlib-1.2.8.tar.gz.ok
    fi

    # http://ftp.gnu.org/pub/gnu/gettext/
    simplebuild ${DEPS_ARCHIVE} gettext-0.18.3.2.tar.gz \
        CFLAGS="-O3 -fPIC" --disable-java

    # http://xmlsoft.org/sources/
    simplebuild ${DEPS_ARCHIVE} libxml2-2.9.1.tar.gz --without-python

    # http://sourceforge.net/projects/libpng/files/
    simplebuild ${DEPS_ARCHIVE} libpng-1.6.7.tar.gz \
        CPPFLAGS="-I${PREFIX}/include" LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib"

    # http://sourceforge.net/projects/expat/files/
    simplebuild ${DEPS_ARCHIVE} expat-2.1.0.tar.gz

    # http://download.savannah.gnu.org/releases/freetype/
    simplebuild ${DEPS_ARCHIVE} freetype-2.5.2.tar.gz CFLAGS="-O3 -fPIC"

    # http://www.freedesktop.org/software/fontconfig/release/
    simplebuild ${DEPS_ARCHIVE} fontconfig-2.11.0.tar.gz CFLAGS="-O3 -fPIC"
    
    # http://cairographics.org/releases/
    simplebuild ${DEPS_ARCHIVE} pixman-0.32.4.tar.gz \
        CFLAGS="-O3 -fPIC" \
        --disable-static-testprogs \
        --disable-gtk        

    # http://cairographics.org/releases/
    simplebuild ${DEPS_ARCHIVE} cairo-1.12.16.tar.xz \
        --enable-xlib=no --enable-xlib-render=no --enable-win32=no  \
        CFLAGS="-O3 -fPIC" CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib"

    # http://sourceware.org/libffi/
    simplebuild ${DEPS_ARCHIVE} libffi-3.2.1.tar.gz \
        CFLAGS="-O3 -fPIC" \
        CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib"

    # glib needs python 2.5 or newer
    ${SDBS_SCRIPTS_DIR}/check_or_build_python2_5.sh
    
    # http://ftp.gnome.org/pub/gnome/sources/glib/
    simplebuild ${DEPS_ARCHIVE} glib-2.42.0.tar.xz \
        CFLAGS="-O3 -fPIC" \
        CPPFLAGS="-I${PREFIX}/include" \
        LDFLAGS="${R}${PREFIX}/lib -L${PREFIX}/lib"

    # http://ftp.gnome.org/pub/GNOME/sources/pango/
    simplebuild ${DEPS_ARCHIVE} pango-1.28.4.tar.bz2 \
        --without-x CFLAGS="-O3 -fPIC"

    simplebuild http://oss.oetiker.ch/rrdtool/pub/ \
        rrdtool-${RRDTOOL_VER}.tar.gz \
        --with-perl-options="PREFIX=$PREFIX LIB=$PREFIX/lib/perl5"
}



# Local Variables:
# mode: shell-script
# indent-tabs-mode: nil
# End:
