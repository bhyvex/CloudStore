g++ -pg -c NameSpace_test.cpp
g++ -pg -g2 -O0  NameSpace_test.o ../../../build/src/storage/NameSpace.o ../../../build/src/storage/FSNameSpace.o  ../../../build/src/storage/KVNameSpace.o   -o namespace_test 
