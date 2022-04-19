dnl config.m4 for extension pfriso

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([pfriso],
dnl  [for pfriso support],
dnl  [AS_HELP_STRING([--with-pfriso],
dnl  [Include pfriso support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([pfriso],
  [whether to enable pfriso support],
  [AS_HELP_STRING([--enable-pfriso],
  [Enable pfriso support])],
  [no])

PFRISO_FILES="pfriso.c friso/src/friso_array.c friso/src/friso_ctype.c friso/src/friso_GBK.c friso/src/friso_hash.c friso/src/friso_lexicon.c friso/src/friso_link.c friso/src/friso_string.c friso/src/friso_UTF8.c friso/src/friso.c"

if test "$PHP_PFRISO" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, PFRISO_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-pfriso -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"  # you might want to change this
  dnl SEARCH_FOR="/include/pfriso.h"  # you most likely want to change this
  dnl if test -r $PHP_PFRISO/$SEARCH_FOR; then # path given as parameter
  dnl  PFRISO_DIR=$PHP_PFRISO
  dnl else # search default path list
  dnl  AC_MSG_CHECKING([for pfriso files in default path])
  dnl  for i in $SEARCH_PATH ; do
  dnl  if test -r $i/$SEARCH_FOR; then
  dnl    PFRISO_DIR=$i
  dnl    AC_MSG_RESULT(found in $i)
  dnl  fi
  dnl  done
  dnl fi
  dnl
  dnl if test -z "$PFRISO_DIR"; then
  dnl  AC_MSG_RESULT([not found])
  dnl  AC_MSG_ERROR([Please reinstall the pfriso distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-pfriso -> add include path
  dnl PHP_ADD_INCLUDE($PFRISO_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-pfriso -> check for lib and symbol presence
  dnl LIBNAME=PFRISO # you may want to change this
  dnl LIBSYMBOL=PFRISO # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl  AC_DEFINE(HAVE_PFRISO_FEATURE, 1, [ ])
  dnl ],[
  dnl  AC_MSG_ERROR([FEATURE not supported by your pfriso library.])
  dnl ], [
  dnl  $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl  PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PFRISO_DIR/$PHP_LIBDIR, PFRISO_SHARED_LIBADD)
  dnl  AC_DEFINE(HAVE_PFRISO_FEATURE, 1, [ ])
  dnl ],[
  dnl  AC_MSG_ERROR([FEATURE not supported by your pfriso library.])
  dnl ],[
  dnl  -L$PFRISO_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PFRISO_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_PFRISO, 1, [ Have pfriso support ])

  # SEARCH_PATH="/usr/local /usr"  # you might want to change this
  # SEARCH_FOR="/include/libfriso.so"  # you most likely want to change this
  # if test -r $PHP_PFRISO/$SEARCH_FOR; then # path given as parameter
  #   PFRISO_DIR=$PHP_PFRISO
  # else # search default path list
  #   AC_MSG_CHECKING([for pfriso files in default path])
  #   for i in $SEARCH_PATH ; do
  #   if test -r $i/$SEARCH_FOR; then
  #    PFRISO_DIR=$i
  #    AC_MSG_RESULT(found in $i)
  #   fi
  #   done
  # fi
  
  # if test -z "$PFRISO_DIR"; then
  #   AC_MSG_RESULT([not found])
  #   AC_MSG_ERROR([Please reinstall the pfriso distribution])
  # fi

  # # Remove this code block if the library supports pkg-config.
  # # --with-pfriso -> add include path
  # PHP_ADD_INCLUDE($PFRISO_DIR/include)

  # # Remove this code block if the library supports pkg-config.
  # # --with-pfriso -> check for lib and symbol presence
  # LIBNAME=PFRISO # you may want to change this
  # LIBSYMBOL=PFRISO # you most likely want to change this

  # # If you need to check for a particular library function (e.g. a conditional
  # # or version-dependent feature) and you are using pkg-config:
  # PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  # [
  #   AC_DEFINE(HAVE_PFRISO_FEATURE, 1, [ ])
  # ],[
  #   AC_MSG_ERROR([FEATURE not supported by your pfriso library.])
  # ], [
  #   $LIBFOO_LIBS
  # ])

  # PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  # [
  #   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PFRISO_DIR/$PHP_LIBDIR, PFRISO_SHARED_LIBADD)
  #   AC_DEFINE(HAVE_PFRISO_FEATURE, 1, [ ])
  # ],[
  #   AC_MSG_ERROR([FEATURE not supported by your pfriso library.])
  # ],[
  #   -L$PFRISO_DIR/$PHP_LIBDIR -lm
  # ])
  
  # PHP_SUBST(PFRISO_SHARED_LIBADD)

  PHP_SUBST(PFRISO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(pfriso, ${PFRISO_FILES}, $ext_shared)
  PHP_ADD_BUILD_DIR([$ext_builddir/friso/src])
  PHP_INSTALL_HEADERS([ext/yaconf], [php_yaconf.h])
fi
