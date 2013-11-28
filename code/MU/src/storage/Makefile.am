#lib_LTLIBRARIES = libstorage.la
noinst_LTLIBRARIES = libstorage.la

libstorage_la_SOURCES = Channel.cpp  \
						ChannelMappingStrategy.cpp \
						HashMappingStrategy.cpp \
						NameSpace.cpp \
						FSNameSpace.cpp \
						KVNameSpace.cpp \
						StoreEngine.cpp  \
						LevelDBEngine.cpp 
						

INCLUDES = $(GLOBAL_INCLUDES)  \
		   -I$(top_srcdir)/common  \
		   -I$(top_srcdir)/src

