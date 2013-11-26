server: AppProto.h EchoAgent.h server.cpp
	g++ -g -I../.. ../../comm/*.cpp ../../log/*.cpp ../../DevLog/*.cpp ../../sys/*.cpp ../../util/*.cpp server.cpp -o server -lpthread -lrt

client: AppProto.h ClientAgent.h client.cpp
	g++ -g -I../.. ../../comm/*.cpp ../../log/*.cpp ../../DevLog/*.cpp ../../sys/*.cpp ../../util/*.cpp client.cpp -o client -lpthread -lrt

clean:
	rm server client
