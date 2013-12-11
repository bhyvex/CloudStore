BINS=CreateFiles DeleteFiles MakeDirs RemoveDirs WriteLargeFiles ReadLargeFiles WriteSmallFiles ReadSmallFiles  \
	TimeWriteSmallFiles TimeWriteSmallFilesFsync TimeWriteSmallFilesAlloc WriteLatency

all: $(BINS)

CreateFiles: CreateFiles.o 
	$(CXX) -o $@ $? -lpthread

DeleteFiles: DeleteFiles.o 
	$(CXX) -o $@ $? -lpthread

MakeDirs: MakeDirs.o 
	$(CXX) -o $@ $? -lpthread

RemoveDirs: RemoveDirs.o 
	$(CXX) -o $@ $? -lpthread

WriteLargeFiles: WriteLargeFiles.o 
	$(CXX) -o $@ $? -lpthread

ReadLargeFiles: ReadLargeFiles.o 
	$(CXX) -o $@ $? -lpthread

WriteSmallFiles: WriteSmallFiles.o 
	$(CXX) -o $@ $? -lpthread

ReadSmallFiles: ReadSmallFiles.o 
	$(CXX) -o $@ $? -lpthread

TimeWriteSmallFiles: TimeWriteSmallFiles.o TimeCounter.o
	$(CXX) -o $@ $? -lpthread

TimeWriteSmallFilesFsync: TimeWriteSmallFilesFsync.o TimeCounter.o
	$(CXX) -o $@ $? -lpthread

TimeWriteSmallFilesAlloc: TimeWriteSmallFilesAlloc.o TimeCounter.o
	$(CXX) -o $@ $? -lpthread

WriteLatency: WriteLatency.o TimeCounter.o
	$(CXX) -o $@ $? -lpthread

clean:
	rm -rf $(BINS) *.o *~

