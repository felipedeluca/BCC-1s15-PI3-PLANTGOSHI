#Pasta com as .h
IDIR=-I include

#Pasta com as .c
SDIR=src

#Compilador
CC=gcc

CFLAGS= -std=c99

LIBS=-L/usr/local/lib -lm -lopencv_core -lopencv_highgui -lallegro -lallegro_main -lallegro_image -lallegro_ttf -lallegro_font -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_dialog

# todas as minhas .c
_SRC=calibra_cores.c camera.c image_proc.c arduino-serial-lib.c plantgoshi_arduino.c
#_SRC=main.c camera.c image_proc.c arduino-serial-lib.c plantgoshi_arduino.c random.c arvore.c

SRC=$(patsubst %,$(SDIR)/%,$(_SRC))

#nome do meu exe
#EXECUTABLE=plantgoshi
EXECUTABLE=calibra_cores

all:
	$(CC) -Wall $(CFLAGS) $(IDIR) $(SRC) -o $(EXECUTABLE) $(LIBS)
