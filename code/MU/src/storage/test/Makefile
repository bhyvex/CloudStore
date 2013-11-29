leveldb:
	g++ -pg -c leveldb_test.cpp
	g++ -pg -g2 -O0  leveldb_test.o ../../../build/src/storage/StoreEngine.o ../../../build/src/storage/LevelDBEngine.o /usr/lib/libleveldb.a -o leveldbtest -lpthread


namespace:
	g++ -pg -c NameSpace_test.cpp
	g++ -pg -g2 -O0  NameSpace_test.o ../../../build/src/storage/NameSpace.o ../../../build/src/storage/FSNameSpace.o  ../../../build/src/storage/KVNameSpace.o   -o namespace_test


clean:
	rm *.o
	rm fs_test -rf
	rm namespace_test leveldbtest
