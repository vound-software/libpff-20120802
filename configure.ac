AC_PREREQ( 2.59 )

AC_INIT(
 [libpff],
 [20120802],
 [joachim.metz@gmail.com])

AC_CONFIG_SRCDIR(
 [include/libpff.h.in])

AM_INIT_AUTOMAKE([gnu 1.6])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL

dnl Check for libtool
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Check for pkg-config
AC_PATH_PROG(PKGCONFIG,[pkg-config])

dnl Support of internationalization (i18n)
AM_GNU_GETTEXT([external])

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Check if WINAPI support should be enabled
AX_COMMON_CHECK_ENABLE_WINAPI

dnl Check if wide character type should be enabled
AX_COMMON_CHECK_ENABLE_WIDE_CHARACTER_TYPE

dnl Check for type definitions
AX_TYPES_CHECK_LOCAL

dnl Checks for libraries and library functions
AX_COMMON_CHECK_LOCAL

dnl Check for enabling libcstring for cross-platform C string functions
AX_LIBCSTRING_CHECK_ENABLE

dnl Check for enabling libcerror for cross-platform C error functions
AX_LIBCERROR_CHECK_ENABLE

dnl Check for enabling libclocale for cross-platform C locale functions
AX_LIBCLOCALE_CHECK_ENABLE

dnl Check for enabling libcnotify for cross-platform C notification functions
AX_LIBCNOTIFY_CHECK_ENABLE

dnl Check for enabling libcsplit for cross-platform C split string functions
AX_LIBCSPLIT_CHECK_ENABLE

dnl Check for enabling libuna for Unicode string support
AX_LIBUNA_CHECK_ENABLE

dnl Check for enabling libcfile for cross-platform C file functions
AX_LIBCFILE_CHECK_ENABLE

dnl Check for enabling libcpath for cross-platform C path functions
AX_LIBCPATH_CHECK_ENABLE

dnl Check for enabling libbfio for basic file IO support
AX_LIBBFIO_CHECK_ENABLE

dnl Check for enabling libfcache for file value cache support
AX_LIBFCACHE_CHECK_ENABLE

dnl Check for enabling libfdata for file data types support
AX_LIBFDATA_CHECK_ENABLE

dnl Check for enabling libfdatetime for file date and time support
AX_LIBFDATETIME_CHECK_ENABLE

dnl Check for enabling libfguid for file GUID/UUID support
AX_LIBFGUID_CHECK_ENABLE

dnl Check for enabling libfvalue for file value support
AX_LIBFVALUE_CHECK_ENABLE

dnl Check for enabling libfwnt for file Windows NT type support
AX_LIBFWNT_CHECK_ENABLE

dnl Check for enabling libfmapi for file MAPI types support
AX_LIBFMAPI_CHECK_ENABLE

dnl Check for library function support
AX_LIBPFF_CHECK_LOCAL

dnl Check for enabling libcsystem for cross-platform C generic system functions
AX_LIBCSYSTEM_CHECK_ENABLE

dnl Check if pfftools should be build as static executables
AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES

dnl Check if libpff should be build with verbose output
AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT

dnl Check if libpff should be build with debug output
AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT

dnl Check if libpff java bindings (jpff) should be build
AX_JAVA_CHECK_ENABLE

dnl Check if libpff python bindings (pypff) should be build
AX_PYTHON_CHECK_ENABLE

dnl Check if DLL support is needed in pfftools
AS_IF(
 [test x"$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
 [AS_CASE(
  [$host],
  [*cygwin* | *mingw*],
  [AC_SUBST(
   [LIBPFF_DLL_IMPORT],
   ["-DLIBPFF_DLL_IMPORT"])
  ])
 ])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Set memwatch support
ac_cv_enable_memwatch=no

AS_IF(
 [test "x$ac_cv_enable_memwatch" != xno ],
 [AC_SUBST(
  [MEMWATCH_CPPFLAGS],
  ["-DMEMWATCH -DMEMWATCH_STDIO"])
 AC_SUBST(
  [MEMWATCH_LIBADD],
  [../common/libmemwatch.la]) ])

AM_CONDITIONAL(
 [HAVE_MEMWATCH],
 [test "x$ac_cv_enable_memwatch" != xno])

dnl Check if requires and build requires should be set in spec file
AS_IF(
 [test "x$ac_cv_libcstring" = xyes || test "x$ac_cv_libcerror" = xyes || test "x$ac_cv_libclocale" = xyes || test "x$ac_cv_libcnotify" = xyes || test "x$ac_cv_libcsplit" = xyes || test "x$ac_cv_libuna" = xyes || test "x$ac_cv_libcfile" = xyes || test "x$ac_cv_libcpath" = xyes || test "x$ac_cv_libbfio" = xyes || test "x$ac_cv_libfcache" = xyes || test "x$ac_cv_libfdata" = xyes || test "x$ac_cv_libfdatetime" = xyes || test "x$ac_cv_libfguid" = xyes || test "x$ac_cv_libfvalue" = xyes || test "x$ac_cv_libfwnt" = xyes || test "x$ac_cv_libfmapi" = xyes],
 [AC_SUBST(
  [libpff_spec_requires],
  [Requires:])
 AC_SUBST(
  [libpff_spec_build_requires],
  [BuildRequires:])
 ])

AS_IF(
 [test "x$ac_cv_libcsystem" = xyes],
 [AC_SUBST(
  [libpff_tools_spec_build_requires],
  [BuildRequires:])
 ])

dnl Set the date for dpkg files
AC_SUBST(
 [DATE],
 [`date -R 2> /dev/null`])

dnl Have configure make the Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcstring/Makefile])
AC_CONFIG_FILES([libcerror/Makefile])
AC_CONFIG_FILES([libclocale/Makefile])
AC_CONFIG_FILES([libcnotify/Makefile])
AC_CONFIG_FILES([libcsplit/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libcfile/Makefile])
AC_CONFIG_FILES([libcpath/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libfcache/Makefile])
AC_CONFIG_FILES([libfdata/Makefile])
AC_CONFIG_FILES([libfdatetime/Makefile])
AC_CONFIG_FILES([libfguid/Makefile])
AC_CONFIG_FILES([libfvalue/Makefile])
AC_CONFIG_FILES([libfwnt/Makefile])
AC_CONFIG_FILES([libfmapi/Makefile])
AC_CONFIG_FILES([libpff/Makefile])
AC_CONFIG_FILES([libcsystem/Makefile])
AC_CONFIG_FILES([pfftools/Makefile])
AC_CONFIG_FILES([jpff/Makefile])
AC_CONFIG_FILES([pypff/Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_FILES([po/Makevars])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([examples/Makefile])
AC_CONFIG_FILES([macosx/Makefile])
AC_CONFIG_FILES([msvscpp/Makefile])
AC_CONFIG_FILES([contrib/Makefile])
dnl Have configure make the include files
AC_CONFIG_FILES([include/libpff.h])
AC_CONFIG_FILES([include/libpff/definitions.h])
AC_CONFIG_FILES([include/libpff/features.h])
AC_CONFIG_FILES([include/libpff/types.h])
AC_CONFIG_FILES([libpff/libpff_definitions.h])
dnl Have configure make distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([dpkg/changelog])
AC_CONFIG_FILES([dpkg/copyright])
AC_CONFIG_FILES([libpff/libpff.rc])
AC_CONFIG_FILES([macosx/Introduction.rtf])
AC_CONFIG_FILES([libpff.pc])
AC_CONFIG_FILES([libpff.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libcstring support:                       $ac_cv_libcstring
   libcerror support:                        $ac_cv_libcerror
   libclocale support:                       $ac_cv_libclocale
   libcnotify support:                       $ac_cv_libcnotify
   libcsplit support:                        $ac_cv_libcsplit
   libuna support:                           $ac_cv_libuna
   libcfile support:                         $ac_cv_libcfile
   libcpath support:                         $ac_cv_libcpath
   libbfio support:                          $ac_cv_libbfio
   libfcache support:                        $ac_cv_libfcache
   libfdata support:                         $ac_cv_libfdata
   libfdatetime support:                     $ac_cv_libfdatetime
   libfguid support:                         $ac_cv_libfguid
   libfvalue support:                        $ac_cv_libfvalue
   libfwnt support:                          $ac_cv_libfwnt
   libfmapi support:                         $ac_cv_libfmapi
   libcsystem support:                       $ac_cv_libcsystem

Features:
   Wide character type support:              $ac_cv_enable_wide_character_type
   pfftools are build as static executables: $ac_cv_enable_static_executables
   Java (jpff) support:                      $ac_cv_enable_java
   Python (pypff) support:                   $ac_cv_enable_python
   Verbose output:                           $ac_cv_enable_verbose_output
   Debug output:                             $ac_cv_enable_debug_output
]);


