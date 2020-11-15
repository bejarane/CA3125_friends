##si copian el del telegram va con error de*** missing separator.  Stop.

CC = g++
SOURCE = clases
OBJ = rodemmanuel.o
OUT = output.bin
OUTDIR = bin
CFLAGS = -I./$(SOURCE) -c -Wall

MKDIR = mkdir -p


edit: $(OBJ)
	$(MKDIR) $(OUTDIR)
	$(CC) -o $(OUTDIR)/$(OUT) $(OBJ)

rodemmanuel.o: rodemmanuel.cpp
	$(CC) $(CFLAGS) rodemmanuel.cpp

.PHONY: clean
clean:
	rm $(OUTDIR)/$(OUT) $(OBJ)
	rmdir $(OUTDIR)

run:
	$(OUTDIR)/$(OUT)