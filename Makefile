all: writetoserial

writetoserial: writetoserial.c
	$(CC) -Wall writetoserial.c -o writetoserial

clean:
	rm writetoserial
