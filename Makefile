#Pasta com as .h
IDIR=-I include

#Pasta com as .c
SDIR=src

#Compilador
CC=gcc

CFLAGS= -std=c99

LIBS=-L/usr/local/lib -lm -lallegro -lallegro_main -lallegro_image -lallegro_ttf -lallegro_font -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_dialog

# todas as minhas .c
_SRC=teste_arvore.c arduino-serial-lib.c plantgoshi_arduino.c arvore.c random.c

SRC=$(patsubst %,$(SDIR)/%,$(_SRC))

#nome do meu exe
EXECUTABLE=arvore

all:
	$(CC) -Wall $(CFLAGS) $(IDIR) $(SRC) -o $(EXECUTABLE) $(LIBS)
