# 
# makefile written by William Halsey
# whalsey@vols.utk.edu
#

CC = gcc
EXEC = test_cd
SRC_C = mkfs.c fs.c mkdir.c tree.c cd.c
SRC_O = mkfs.o fs.o mkdir.o tree.o cd.o
FLAGS = -std=c99


all: clear_screen obj
	$(CC) $(FLAGS) $(SRC_O) $(EXEC).c -o $(EXEC).e

debug: clear_screen obj
	$(CC) $(FLAGS) -g $(SRC_O) $(EXEC).c -o $(EXEC)
	
obj:
	$(CC) $(FLAGS) $(SRC_C) -c

clean:
	rm *.o *.e

clear_screen: 
	clear
