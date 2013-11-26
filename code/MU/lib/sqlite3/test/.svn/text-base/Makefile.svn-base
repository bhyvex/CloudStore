CXX=g++
CXX_FLAGS=
SQLITE3_HOME=/home/sw/app/sqlite3

simpleTest:
	$(CXX) $(CXX_FLAGS) -I$(SQLITE3_HOME)/include simpleTest.cpp $(SQLITE3_HOME)/lib/libsqlite3.a -o simpleTest -ldl -lpthread
	#$(CXX) $(CXX_FLAGS) -I$(SQLITE3_HOME)/include -L$(SQLITE3_HOME)/lib -lsqlite3 simpleTest.cpp -o simpleTest
