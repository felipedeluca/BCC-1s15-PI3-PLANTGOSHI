#ifndef PLANTGOSHI_ARDUINO_H
#define PLANTGOSHI_ARDUINO_H

#include <stdlib.h>
// Configuração

#define MAX_BUFFER 256

typedef struct {

    int baudRate;// = 9600;
    int timeOut; // = 5000;
    int fd;      // = -1;
    int rc, n;

    char serialPort[ MAX_BUFFER ]; // Informar o caminho completo da porta serial no Linux ou Mac OSX
    char buffer[ MAX_BUFFER ]; // Armazena os valores das cores
    char endOfLine; // = '\n';

} ArduinoComm;

void arduino_LED( ArduinoComm* comm );

#endif
