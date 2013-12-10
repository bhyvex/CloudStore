all: string kvnamespace namespace leveldb  

leveldb:
	g++ -pg -c leveldb_test.cpp
	g++ -pg -g2 -O0  leveldb_test.o \
			../../../build/src/storage/StoreEngine.o \
			../../../build/src/storage/LevelDBEngine.o \
			../../../../../third_party/leveldb-1.9.0/libleveldb.a \
		-o leveldb_test -lpthread -lsnappy


namespace:
	g++ -pg -c NameSpace_test.cpp  -I../../../src/
	g++ -pg -g2 -O0  NameSpace_test.o \
			../../../build/src/storage/NameSpace.o \
			../../../build/src/storage/FSNameSpace.o  \
			../../../build/src/storage/KVNameSpace.o  \
		-o namespace_test -I../../../src/


kvnamespace:
	g++ -pg -c KVnameSpace_test.cpp  -I../../../src/
	g++ -pg -g2 -O0  KVnameSpace_test.o \
			../../../build/src/storage/NameSpace.o \
			../../../build/src/storage/KVNameSpace.o  \
		-o kvnamespace_test -I../../../src/


string:
	g++ -pg  string.cpp -o string


clean:
	rm gmon.out *.o string namespace_test kvnamespace_test leveldb_test data.db leveldbDir fs_test -rf
