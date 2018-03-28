
CC = gcc
CFLAGS = -Wall -g
OBJFILES = attribute.o classfile.o error.o infile.o jstring.o output.o parsedescr.o parse_opt.o

hjavap: $(OBJFILES) main.c defs.h classfile.h output.h
	$(CC) $(CFLAGS) -o hjavap $(OBJFILES) main.c

attribute.o: attribute.c attribute.h classfile.h infile.h
	$(CC) $(CFLAGS) -c attribute.c

classfile.o: classfile.c classfile.h attribute.h infile.h jstring.h
	$(CC) $(CFLAGS) -c classfile.c

error.o: error.c defs.h
	$(CC) $(CFLAGS) -c error.c

infile.o: infile.c defs.h
	$(CC) $(CFLAGS) -c infile.c

jstring.o: jstring.c jstring.h
	$(CC) $(CFLAGS) -c jstring.c

output.o: output.c output.h classfile.h
	$(CC) $(CFLAGS) -c output.c

parsedescr.o: parsedescr.c
	$(CC) $(CFLAGS) -c parsedescr.c

parse_opt.o: parse_opt.c
	$(CC) $(CFLAGS) -c parse_opt.c

clean:
	rm *.c~ *.h~


