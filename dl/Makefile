
CFLAGS=-Wall -Werror

main: main.c
	gcc main.c $(CFLAGS) -o $@

libwrapper.so: wrapper.c
	gcc wrapper.c -shared -o $@ -ldl
