# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort readPrintInt readPrintCharString sub ascii bubblesort help Create_open_close ping pong scheduler sinhvien voinuoc MayRotNuoc

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

sub.o: sub.c
	$(CC) $(CFLAGS) -c sub.c
sub: sub.o start.o
	$(LD) $(LDFLAGS) start.o sub.o -o sub.coff
	../bin/coff2noff sub.coff sub


readPrintInt.o: readPrintInt.c
	$(CC) $(CFLAGS) -c readPrintInt.c
readPrintInt: readPrintInt.o start.o
	$(LD) $(LDFLAGS) start.o readPrintInt.o -o readPrintInt.coff
	../bin/coff2noff readPrintInt.coff readPrintInt

readPrintCharString.o: readPrintCharString.c
	$(CC) $(CFLAGS) -c readPrintCharString.c
readPrintCharString: readPrintCharString.o start.o
	$(LD) $(LDFLAGS) start.o readPrintCharString.o -o readPrintCharString.coff
	../bin/coff2noff readPrintCharString.coff readPrintCharString

ascii.o: ascii.c
	$(CC) $(CFLAGS) -c ascii.c
ascii: ascii.o start.o
	$(LD) $(LDFLAGS) start.o ascii.o -o ascii.coff
	../bin/coff2noff ascii.coff ascii

bubblesort.o: bubblesort.c
	$(CC) $(CFLAGS) -c bubblesort.c
bubblesort: bubblesort.o start.o
	$(LD) $(LDFLAGS) start.o bubblesort.o -o bubblesort.coff
	../bin/coff2noff bubblesort.coff bubblesort

help.o: help.c
	$(CC) $(CFLAGS) -c help.c
help: help.o start.o
	$(LD) $(LDFLAGS) start.o help.o -o help.coff
	../bin/coff2noff help.coff help

Create_open_close.o: Create_open_close.c
	$(CC) $(CFLAGS) -c Create_open_close.c
Create_open_close: Create_open_close.o start.o
	$(LD) $(LDFLAGS) start.o Create_open_close.o -o Create_open_close.coff
	../bin/coff2noff Create_open_close.coff Create_open_close


ping.o: ping.c
	$(CC) $(CFLAGS) -c ping.c
ping: ping.o start.o
	$(LD) $(LDFLAGS) start.o ping.o -o ping.coff
	../bin/coff2noff ping.coff ping

pong.o: pong.c
	$(CC) $(CFLAGS) -c pong.c
pong: pong.o start.o
	$(LD) $(LDFLAGS) start.o pong.o -o pong.coff
	../bin/coff2noff pong.coff pong

scheduler.o: scheduler.c
	$(CC) $(CFLAGS) -c scheduler.c
scheduler: scheduler.o start.o
	$(LD) $(LDFLAGS) start.o scheduler.o -o scheduler.coff
	../bin/coff2noff scheduler.coff scheduler


sinhvien.o: sinhvien.c
	$(CC) $(CFLAGS) -c sinhvien.c
sinhvien: sinhvien.o start.o
	$(LD) $(LDFLAGS) start.o sinhvien.o -o sinhvien.coff
	../bin/coff2noff sinhvien.coff sinhvien

voinuoc.o: voinuoc.c
	$(CC) $(CFLAGS) -c voinuoc.c
voinuoc: voinuoc.o start.o
	$(LD) $(LDFLAGS) start.o voinuoc.o -o voinuoc.coff
	../bin/coff2noff voinuoc.coff voinuoc


MayRotNuoc.o: MayRotNuoc.c
	$(CC) $(CFLAGS) -c MayRotNuoc.c
MayRotNuoc: MayRotNuoc.o start.o
	$(LD) $(LDFLAGS) start.o MayRotNuoc.o -o MayRotNuoc.coff
	../bin/coff2noff MayRotNuoc.coff MayRotNuoc










