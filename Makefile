CC = gcc
EXE_FILENAME = client server

all:	$(EXE_FILENAME)	

client:	client.c
	$(CC) -g -o $@ $^
server:	server.c
	$(CC) -g -o $@ $^

clean:
	rm -rf $(EXE_FILENAME)
