.PHONY: all, clean
all:
	gcc -g -Wall server.c -o httpd -lpthread
clean:
	rm httpd
