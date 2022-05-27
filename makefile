all:  server client test
server: server.o stack.a stack.hpp
	g++ -Wall -g -o server server.o stack.a 

test: test.o stack.a stack.hpp
	g++ -Wall -g -o test test.o stack.a 

stack: stack.a

stack.a: stack.o
	ar -rcs stack.a stack.o 

client: client.cpp 
	g++ -Wall -g client.cpp -o client 

server.o: server.cpp stack.hpp
	g++ -Wall -g -c server.cpp

stack.o: stack.cpp stack.hpp
	g++ -Wall -g -c stack.cpp

test.o: stack.hpp
	g++ -Wall -g -c test.cpp -lpthread
.PHONY: clean#.PHONY means that clean is not a file.
clean:
	rm -f *.o *.a  server client test