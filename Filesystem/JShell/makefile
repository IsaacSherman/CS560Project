# CS360 Class Manager
# August 29, 2005
CC = gcc

MYLIB = src/
INCLUDE = -I$(MYLIB)
OTHER = $(wildcard $(MYLIB)*.c)
SOURCE = $(wildcard *.c)
ALLC = $(OTHER) $(SOURCE)
COMPS = $(subst .c,.o, $(OTHER))
OBJECTS =  $(subst .c,.o, $(OTHER)) $(subst .c,.o, $(SOURCE))
EXECUTABLES = $(basename $(SOURCE))
CFLAGS =  -g $(INCLUDE)
LIB = 
# $(CDIR)/pub/libfdr.a
all: $(OBJECTS) $(EXECUTABLES)
	echo ALL:
test: 
	echo Comps == $(COMPS)
	echo oBjects = $(OBJECTS)
	echo SOURCE = $(SOURCE)
	echo OTHER = $(OTHER)
	
clean:
	rm -f core $(EXECUTABLES) *.o

$(EXECUTABLES) : $(LIB) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIB)

#.SUFFIXES: .c .o .a

$(ALLC) : $(LIB)
	$(CC) -c $(CFLAGS) $*.c -o $(basename $@).o $(COMPS)
	echo ALLC: $@
	$(CC) -c $(CFLAGS) $@ $(COMPS)

#lib/f1.o:
#	$(CC) $(CFLAGS) $(basename $@).c -o $@ $(LIB)
$(OBJECTS) : $(ALLC)
	echo OBJECT: $@
	$(CC) -c $(CFLAGS) $(basename $@).c -o $@


