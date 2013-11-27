#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([mu], [2.0], [shiwei2012@gmail.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(mu, 2.0)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
#AC_PROG_RANLIB
AC_PROG_LIBTOOL

# enable debug or not
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [enable debug mode(default=no)]),
                [],
                [enable_debug=no])
AS_IF([test "x$enable_debug" = "xyes"], [CXXFLAGS="-g2 -O0 -DDEBUG -Wall"],
      [test "x$enable_debug" = "xno"], [CXXFLAGS="-O2 -Wall"],
      [])

# with google protocol buffer
AC_ARG_WITH(protobuf, AS_HELP_STRING([--with-protobuf], [prefix for installed protobuf lib]),
                [],
                [])


# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [dlopen], [], [echo "dl not found" && exit 1])
# FIXME: Replace `main' with a function in `-lprotobuf':
AC_CHECK_LIB([protobuf], [_ZN6google8protobuf11MessageLite15ParseFromStringERKSs], [], [echo "protobuf not found" && exit 1])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [pthread_create], [], [echo "pthread not found" && exit 1])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [clock_gettime], [], [echo "rt not found" && exit 1])
# FIXME: Replace `main' with a function in `-lxml2':
AC_CHECK_LIB([xml2], [xmlNewDoc], [], [echo "libxml2 not found" && exit 1])
AC_CHECK_LIB([zip], [zip_open], [], [echo "libzip not found" && exit 1])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h limits.h malloc.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/file.h sys/ioctl.h sys/mount.h sys/param.h sys/socket.h sys/statvfs.h sys/time.h unistd.h utime.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([bzero clock_gettime fdatasync ftruncate gettimeofday inet_ntoa localtime_r memmove memset mkdir munmap pow rmdir socket strchr strerror utime])

AC_CONFIG_FILES([Makefile 
                 common/Makefile
                 common/comm/Makefile
                 common/DevLog/Makefile
                 common/sys/Makefile
                 common/log/Makefile
                 common/util/Makefile
                 common/xml/Makefile
                 common/timer/Makefile
                 lib/Makefile
                 lib/sqlite3/Makefile
                 src/Makefile
                 src/ClientPM/Makefile
                 src/CSPM/Makefile
                 src/dao/Makefile
                 src/frame/Makefile
                 src/init/Makefile
                 src/item/Makefile
                 src/MigrationPM/Makefile
                 src/oplog/Makefile
                 src/protocol/Makefile
                 src/state/Makefile
                 src/zip/Makefile
                 ])
AC_OUTPUT
