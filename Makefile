client : client.o
	gcc -o client client.o

server : server.o
	gcc -o server server.o

client.o : client.c
	gcc -c -o client.o client.c

server.o : server.c
	gcc -c -o server.o server.c

clean:
	rm *.o client server

all :
	make client
	make server 
