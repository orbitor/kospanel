CC = gcc
C_FLAGS = -DHAVE_XPM -Wall -I/usr/X11R6/include
L_FLAGS = -L/usr/X11R6/lib -lX11 -lXpm
EXTRA_CFLAGS = -g -Os
PROGNAME = kospanel

$(PROGNAME): Makefile kospanel.c kospanel.h icon.xpm
	$(CC) $(C_FLAGS) $(EXTRA_CFLAGS) kospanel.c -o $(PROGNAME) $(L_FLAGS)

clean:
	rm -f core *.o $(PROGNAME) nohup.out

install: $(PROGNAME)
	install -d -m 755 $(DESTDIR)/usr/bin
	install -m 755 $(PROGNAME) $(DESTDIR)/usr/bin
	install -d -m 755 $(DESTDIR)/usr/share/man/man1

