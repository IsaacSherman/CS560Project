# 
# makefile written by William Halsey
# whalsey@vols.utk.edu
#

CC = gcc
EXEC = test_ls
SRC_C = mkfs.c fs.c mkdir.c tree.c cd.c ls.c rmdir.c import.c cat.c secretCat.c open.c close.c
SRC_O = mkfs.o fs.o mkdir.o tree.o cd.o ls.o rmdir.o import.o cat.o secretCat.o open.o close.o
FLAGS = -std=c99

CFLAGS = -g -O $(FLAGS)


OBJS = fs.o mkdir.o mkfs.o ls.o cd.o tree.o rmdir.o import.o cat.o secretCat.o open.o close.o



lib: $(OBJS)
	ar ru fslib.a $(OBJS)
	ranlib fslib.a 
	cp fslib.a ../../JShell/libfdr/
	

clean:
	rm -f core *.o libfdr.a

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c


all: clear_screen obj
	$(CC) $(FLAGS) $(SRC_O) $(EXEC).c -o $(EXEC).e

debug: clear_screen obj
	$(CC) $(FLAGS) -g $(SRC_O) $(EXEC).c -o $(EXEC).e
	
obj:
	$(CC) $(FLAGS) $(SRC_C) -c

clean:
	rm *.o *.e

clear_screen: 
	clear

	
