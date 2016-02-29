CC = gcc

MYLIB = src/
INCLUDE = -I $(MYLIB) -Ilibfdr/
OTHER = $(wildcard $(MYLIB)*.c)
SOURCE = $(wildcard *.c)
ALLC = $(OTHER) $(SOURCE)
COMPS = $(subst .c,.o, $(OTHER))
OBJECTS =  $(subst .c,.o, $(OTHER)) $(subst .c,.o, $(SOURCE))
EXECUTABLES = $(basename $(SOURCE))
CFLAGS =  -g $(INCLUDE) -std=c99
LIB = libfdr/libfdr.a libfdr/fslib.a


all: $(OBJECTS) $(EXECUTABLES)
	echo ALL: $(OBJECTS) $(EXECUTABLES)
test: 
	echo Comps == $(COMPS)
	echo oBjects = $(OBJECTS)
	echo SOURCE = $(SOURCE)
	echo OTHER = $(OTHER)
	
clean:
	rm -f core $(EXECUTABLES) *.o

$(EXECUTABLES) : $(LIB) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIB)
	clear

#.SUFFIXES: .c .o .a

$(ALLC) : $(LIB)
	$(CC) $(CFLAGS) -c $@ -o $(basename $@).o 
	echo ALLC: $@
	$(CC) -c $(CFLAGS) $@
	clear

#lib/f1.o:
#	$(CC) $(CFLAGS) $(basename $@).c -o $@ $(LIB)
$(OBJECTS) : $(ALLC)
	echo OBJECT: $@
	$(CC) -c $(CFLAGS) $(basename $@).c -o $@
	clear


