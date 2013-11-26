BINS = loadgen

all: $(BINS)

loadgen: loadgen.cpp ../tool/net.cpp
	g++ -D__STDC_FORMAT_MACROS -g -I ../../ -I ../../../common  \
		-I /usr/local/protobuf/include  \
		../../protocol/*.cc $?  \
		-L /usr/local/protobuf/lib -o $@ -lpthread -lprotobuf


clean:
	rm -f $(BINS)
