dnl $Id$
dnl config.m4 for extension pmautomata

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(pmautomata, for pmautomata support,
dnl Make sure that the comment is aligned:
dnl [  --with-pmautomata             Include pmautomata support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(pmautomata, whether to enable pmautomata support,
Make sure that the comment is aligned:
[  --enable-pmautomata           Enable pmautomata support])

CC=/usr/bin/gcc
CXX=/usr/bin/g++
CFLAGS="-O2 -lstdc++"
CXXFLAGS="-O2 -fPIC"

if test "$PHP_PMAUTOMATA" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-pmautomata -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/pmautomata.h"  # you most likely want to change this
  dnl if test -r $PHP_PMAUTOMATA/$SEARCH_FOR; then # path given as parameter
  dnl   PMAUTOMATA_DIR=$PHP_PMAUTOMATA
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for pmautomata files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PMAUTOMATA_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PMAUTOMATA_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the pmautomata distribution])
  dnl fi

  dnl # --with-pmautomata -> add include path
  dnl PHP_ADD_INCLUDE($PMAUTOMATA_DIR/include)

  dnl # --with-pmautomata -> check for lib and symbol presence
  dnl LIBNAME=pmautomata # you may want to change this
  dnl LIBSYMBOL=pmautomata # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PMAUTOMATA_DIR/lib, PMAUTOMATA_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PMAUTOMATALIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong pmautomata lib version or lib not found])
  dnl ],[
  dnl   -L$PMAUTOMATA_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PMAUTOMATA_SHARED_LIBADD)

  PHP_NEW_EXTENSION(pmautomata, pmautomata.cpp, $ext_shared)
fi
