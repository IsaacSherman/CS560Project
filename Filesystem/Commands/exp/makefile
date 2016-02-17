# 
# makefile written by William Halsey
# whalsey@vols.utk.edu
#

CC = gcc
EXEC = test_mkfs
SRC = mkfs
FLAGS = 

all: clear_screen obj
	$(CC) $(FLAGS) $(SRC).o $(EXEC).c -o $(EXEC)

obj:
	$(CC) $(FLAGS) $(SRC).c -c

clean:
	rm *.o $(EXEC)

clear_screen: 
	clear