#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Chess], [2.1], [geoganias@gmail.com])
# Check if exists
AC_CONFIG_SRCDIR([include/ChessInterface.hpp])
# Put Configuration results here
AC_CONFIG_HEADERS([include/config.h])
# Put autotools aux files in a subdir, so they don't clutter top dir: 
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.11 subdir-objects -Wall -Werror])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([termios.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
