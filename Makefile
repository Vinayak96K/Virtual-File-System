obj-m += Myexe.o

all:
	gcc EntryPoint.c HelperFunctions.c CommandHelper.c FileHelper.c
