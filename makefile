##si copian el del telegram va con error de*** missing separator.  Stop.

CC = g++
SOURCE = clases
OBJ = amigos.o
OUT = output.bin
OUTDIR = bin
CFLAGS = -I./$(SOURCE) -c -Wall

MKDIR = mkdir -p


edit: $(OBJ)
	$(MKDIR) $(OUTDIR)
	$(CC) -o $(OUTDIR)/$(OUT) $(OBJ)

amigos.o: amigos.cpp
	$(CC) $(CFLAGS) amigos.cpp

.PHONY: clean
clean:
	rm $(OUTDIR)/$(OUT) $(OBJ)
	rmdir $(OUTDIR)

run:
	$(OUTDIR)/$(OUT)