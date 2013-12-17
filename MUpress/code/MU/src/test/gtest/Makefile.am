noinst_PROGRAMS = DataGen  \
			   GetUserInfoTest  \
			   PutDirTest  \
			   DelDirTest  \
			   GetDirTest  \
			   GetDir2Test  \
			   StatDirTest  \
			   MovDirTest  \
			   PutFileTest  \
			   DelFileTest  \
			   GetFileTest  \
			   MovFileTest

DataGen_SOURCES = DataGen.cpp DataGenMain.cpp

GetUserInfoTest_SOURCES = DataGen.cpp MUTest.cpp GetUserInfoTest.cpp

PutDirTest_SOURCES = DataGen.cpp MUTest.cpp PutDirTest.cpp

DelDirTest_SOURCES = DataGen.cpp MUTest.cpp DelDirTest.cpp

GetDirTest_SOURCES = DataGen.cpp MUTest.cpp GetDirTest.cpp

GetDir2Test_SOURCES = DataGen.cpp MUTest.cpp GetDir2Test.cpp

StatDirTest_SOURCES = DataGen.cpp MUTest.cpp StatDirTest.cpp

MovDirTest_SOURCES = DataGen.cpp MUTest.cpp MovDirTest.cpp

PutFileTest_SOURCES = DataGen.cpp MUTest.cpp PutFileTest.cpp

DelFileTest_SOURCES = DataGen.cpp MUTest.cpp DelFileTest.cpp

GetFileTest_SOURCES = DataGen.cpp MUTest.cpp GetFileTest.cpp

MovFileTest_SOURCES = DataGen.cpp MUTest.cpp MovFileTest.cpp

AM_LDFLAGS = -lgtest

LIBS = $(top_builddir)/common/log/liblog.la  \
	   $(top_builddir)/common/util/libutil.la  \
	   $(top_builddir)/common/xml/libxml.la  \
	   $(top_builddir)/MU/protocol/libprotocol.la

INCLUDES = $(GLOBAL_INCLUDES)  \
		   -I $(top_srcdir)/common  \
		   -I $(top_srcdir)/MU
