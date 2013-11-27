BIN=test
$(BIN): DevLog.o main.o FunctionTrace.o
	g++ -g -lpthread -o $(BIN) DevLog.o main.o FunctionTrace.o
	#./$(BIN)
DevLog.o: ../DevLog.cpp ../DevLog.h
	g++ -g -c -o DevLog.o ../DevLog.cpp
FunctionTrace.o: ../FunctionTrace.cpp ../FunctionTrace.h
	g++ -g -c -o FunctionTrace.o ../FunctionTrace.cpp
main.o: main.cpp
	g++ -g -lpthread -c -o main.o main.cpp
clean:
	-test -z $(BIN)||rm -f $(BIN)
	-test -z logfile||rm -f logfile
	rm -f *.o *~

