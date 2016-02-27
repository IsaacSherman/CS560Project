# 
# makefile written by William Halsey
# whalsey@vols.utk.edu
#

CC = gcc
EXEC = test_mkfs.e
SRC_C = mkfs.c fs.c
SRC_O = mkfs.o fs.o
FLAGS = -std=c99


all: clear_screen obj
	$(CC) $(FLAGS) $(SRC_O) -o $(EXEC)

debug: clear_screen obj
	$(CC) $(FLAGS) -g $(SRC_O) $(EXEC).c -o $(EXEC)
	
obj:
	$(CC) $(FLAGS) $(SRC_C) -c

clean:
	rm *.o $(EXEC)

clear_screen: 
	clear
