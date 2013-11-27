#lib_LTLIBRARIES = liboplog.la
noinst_LTLIBRARIES = liboplog.la

liboplog_la_SOURCES = Connection.cpp  \
					 LogAccessEngine.cpp  \
					 LogDispatcher.cpp  \
					 LogEvent.cpp  \
					 LogRotationTask.cpp  \
					 LogThread.cpp  \
					 Statement.cpp  \
					 LogFlushTask.cpp  

INCLUDES = $(GLOBAL_INCLUDES)  \
		   -I$(top_srcdir)/common  \
		   -I$(top_srcdir)/src  \
		   -I$(top_srcdir)/lib
