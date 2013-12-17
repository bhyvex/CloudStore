BINS = FakeCS FakeRS

all: $(BINS)

FakeCS:
	g++ -g -I ../../ -I ../../../common -I /usr/local/protobuf/include ../../protocol/*.cc FakeCS.cpp -L /usr/local/protobuf/lib -o FakeCS -lpthread -lprotobuf

FakeRS:
	g++ -g -I ../../ -I ../../../common -I /usr/local/protobuf/include -I /usr/include/libxml2 ../../protocol/*.cc ../../../common/xml/*.cpp ../../../common/log/*.cpp FakeRS.cpp -L /usr/local/protobuf/lib -o FakeRS -lpthread -lprotobuf -lxml2

clean:
	rm $(BINS)
