# makefile for Virtual Memeory Unit (MMU)
#
# usage: make mmu 

CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o
	rm -rf mmu256
	rm -rf mmu128

all: mmu256 mmu128
	
mmu256: mmu256.o
	$(CC) $(CFLAGS) -o mmu256 mmu256.o

mmu128: mmu128.o
	$(CC) $(CFLAGS) -o mmu128 mmu128.o


