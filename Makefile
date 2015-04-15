#Pasta com as .h
IDIR=-I include

#Pasta com as .c
SDIR=src

#Compilador
CC=gcc

PKG_CONFIG=-L/usr/local/lib -lallegro -lallegro_main -lallegro_image -lallegro_ttf -lallegro_font -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_dialog

# todas as minhas .c
_SRC=main.c arduino-serial-lib.c

SRC=$(patsubst %,$(SDIR)/%,$(_SRC))

#nome do meu exe
EXECUTABLE=plantgoshi

all:
	gcc $(SRC) -o $(EXECUTABLE) $(PKG_CONFIG) $(IDIR) -std=c99