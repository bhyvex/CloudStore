BINS = zip unzip

all: $(BINS)

zip: zip.cpp
	g++ -g -I../.. ../Zip.cpp $? -o $@ -lzip

unzip: unzip.cpp
	g++ -g -I../.. ../Zip.cpp $? -o $@ -lzip

clean:
	rm $(BINS)
