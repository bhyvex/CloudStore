BINS = Migrate Extent CreateUser DeleteUser ReadFileMeta WriteFileMeta

all: $(BINS)

Migrate: Migrate.cpp
	g++ -D__STDC_FORMAT_MACROS -g -I ../../ -I ../../../common  \
		-I /usr/local/protobuf/include  \
		../../protocol/*.cc $?  \
		-L /usr/local/protobuf/lib -o $@ -lpthread -lprotobuf

Extent: Extent.cpp
	g++ -D__STDC_FORMAT_MACROS -g -I ../../ -I ../../../common  \
		-I /usr/local/protobuf/include  \
		../../protocol/*.cc $?  \
		-L /usr/local/protobuf/lib  \
		-o $@ -lpthread -lprotobuf

CreateUser: CreateUser.cpp
	g++ -D__STDC_FORMAT_MACROS -g -I ../../ -I ../../../common  \
	-I /usr/local/protobuf/include  \
	../../protocol/*.cc $?  \
	-L /usr/local/protobuf/lib  \
	-o $@ -lpthread -lprotobuf

DeleteUser: DeleteUser.cpp
	g++ -D__STDC_FORMAT_MACROS  -g -I ../../ -I ../../../common  \
		-I /usr/local/protobuf/include  \
		../../protocol/*.cc $?  \
		-L /usr/local/protobuf/lib  \
		-o $@ -lpthread -lprotobuf

ReadFileMeta: ReadFileMeta.cpp
	g++ -D__STDC_FORMAT_MACROS -g -I ../../ -I ../../../common  \
		../../../common/util/*.cpp  \
		../../../common/log/*.cpp $? -o $@ -lpthread

WriteFileMeta: WriteFileMeta.cpp
	g++ -D__STDC_FORMAT_MACROS -g -I ../../ -I ../../../common  \
		-I ../../../src  \
		../../../common/util/*.cpp  \
		../../../common/log/*.cpp $? -o $@ -lpthread

clean:
	rm $(BINS)
