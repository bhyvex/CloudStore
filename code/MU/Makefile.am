bin_PROGRAMS = MU

MU_SOURCES = src/frame/main.cpp

MU_LDADD = src/libMU.la  \
		  common/libcommon.la  \
		  lib/sqlite3/libsqlite3.la

AM_CXXFLAGS = -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
export AM_CXXFLAGS

GLOBAL_INCLUDES = -I/usr/local/protobuf/include  \
				  -I/usr/local/include/libxml2  \
				  -I/usr/local/gtest/include  \
				  -I/usr/include
export GLOBAL_INCLUDES 

INCLUDES = $(GLOBAL_INCLUDES)  \
		   -I$(top_srcdir)/common  \
		   -I$(top_srcdir)/src

SUBDIRS = common src lib

