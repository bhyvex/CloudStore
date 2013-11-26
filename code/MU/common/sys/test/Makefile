INCLUDES=-I../../
LDFLAGS=-lpthread -lrt

TARGETS=SimpleTestThread SimpleTestThreadPool

all: $(TARGETS)

SimpleTestThread: SimpleTestThread.cpp
	g++ $(INCLUDES) $(LDFLAGS) $(LDPATH) ../../log/*.cpp ../../sys/*.cpp $? -o$@

SimpleTestThreadPool: SimpleTestThreadPool.cpp
	g++ $(INCLUDES) $(LDFLAGS) $(LDPATH) ../../log/*.cpp ../../sys/*.cpp $? -o$@

clean:
	rm $(TARGETS)
