AC_INIT([visamp],[0.1.0])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
AC_PROG_INSTALL
AC_CACHE_SAVE

my_host=dummy
case $host in
     *-linux-* )
     my_host=linux
     AC_CHECK_LIB([v4l2],[v4l2_open])
     break
     ;;
     *-apple-darwin* )
     my_host=darwin
     break
     ;;
     *-irix* )
     my_host=irix
     break
     ;;
esac

AM_CONDITIONAL([LINUX],  [test x$my_host = xlinux]) 
AM_CONDITIONAL([IRIX],   [test x$my_host = xirix]) 
AM_CONDITIONAL([DARWIN], [test x$my_host = xdarwin]) 
AM_CONDITIONAL([DUMMY],  [test x$my_host = xdummy]) 

PKG_PROG_PKG_CONFIG([0.17])
PKG_CHECK_MODULES([QtCore], [QtCore >= 4.8.4])
PKG_CHECK_MODULES([QtGui], [QtGui >= 4.8.4])
AC_CHECK_PROG(MOC, [moc], [moc])

AC_CACHE_SAVE

# set the libtool shared library version numbers
lt_major=1
lt_revision=0
lt_age=0

AC_SUBST(lt_major)
AC_SUBST(lt_revision)
AC_SUBST(lt_age)
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/libimrec/Makefile])
AC_CONFIG_FILES([src/visamp/Makefile])

AC_OUTPUT
