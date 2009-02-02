AC_INIT([bubblebreak], [0.1], [guillaume@sadegh-beyki.com])

AC_PREREQ([2.57])

AC_CONFIG_AUX_DIR([build-aux])
AM_CONFIG_HEADER(src/config.h)

AM_INIT_AUTOMAKE([1.7.5 dist-bzip2 no-dist-gzip no-installinfo -Wall foreign])

dnl If the user didn't supplied a CFLAGS value,
dnl set an empty one to prevent autoconf to stick -O2 -g here.
test -z $CXXFLAGS && CXXFLAGS=

dnl Checks for programs.
AC_PROG_CXX([g++43 g++42 g++ cc])
AC_PROG_INSTALL
AC_PROG_GCC_TRADITIONAL

dnl Checks for header files.
AC_HEADER_STDC

CXXFLAGS="${CXXFLAGS} -Wall -Wextra -Werror -pedantic `sdl-config --cflags`"
LDFLAGS="$LDFLAGS `sdl-config --libs` -lGL  -lglut -lSDL_image -lGLU"

dnl --with-debug.
AC_ARG_WITH([debug],
  [AS_HELP_STRING([--with-debug],
    [For debugging with gdb])],
  [with_debug=yes;
  echo "compiling in debug mode ... -ggdb3";
  CXXFLAGS="$CXXFLAGS -ggdb3"],
  [CXXFLAGS="${CXXFLAGS} -DNDEBUG"])

dnl AC_CHECK_LIB([termcap], [main], , [AC_ERROR(No working `termcap' found on your system.)])

AC_CONFIG_FILES([
   Makefile
   src/Makefile
])

AC_OUTPUT
