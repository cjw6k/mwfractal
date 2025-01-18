#
#   Copyright 2013 Neil E. Moore, Christopher J. Willcock
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
#

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([mwfractal], [1.0], [cjwillcock@ieee.org])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB([boost_program_options], [main], [], [AC_MSG_ERROR([boost_program_options library not found])])
PKG_CHECK_MODULES([MAGICK], [Magick++], [], [AC_MSG_ERROR([Magick++ library not found])])

# Checks for math library functions.
AC_SEARCH_LIBS([pow], [m], [], [AC_MSG_ERROR([pow not found in the math library])])
AC_SEARCH_LIBS([modf], [m], [], [AC_MSG_ERROR([modf not found in the math library])])
AC_SEARCH_LIBS([floor], [m], [], [AC_MSG_ERROR([floor not found in the math library])])

# Generate output files.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
