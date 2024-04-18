CFLAGS = -g -O -Wall -W -pedantic -std=c99 
LDFLAGS = -L/opt/vc/lib
LDLIBS = -lm -lgmp

rsa: src/main.c src/rsa.h src/rsa.c
	gcc $(CFLAGS) $(LDFLAGS) src/main.c -o bin/rsa $(LDLIBS)
install: src/main.c src/rsa.h src/rsa.c
	gcc $(CFLAGS) $(LDFLAGS) src/main.c -o bin/rsa $(LDLIBS) && cp -f ./bin/rsa $(HOME)/.local/bin/
