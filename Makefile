.PHONY: all, clean
all:
	gcc -g -Wall server.c -o httpd
clean:
	rm httpd
