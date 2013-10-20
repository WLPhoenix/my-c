
my:
	gcc -g -std=c99  -pedantic-errors \-D_GNU_SOURCE \-D__USE_GNU \-o bin/my src/my.c -luuid
	
install:
	cp bin/my /usr/bin/my
