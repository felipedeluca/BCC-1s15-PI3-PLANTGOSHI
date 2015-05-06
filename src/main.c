#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "plantgoshi_arduino.h"
#include "arvore.h"

ArduinoComm arduinoComm;
//------------------------------------------------------------------------------
void setupArduino( void ) {

    // Configura a comunicação com o Arduino
    strcpy( arduinoComm.serialPort, "/dev/cu.usbmodem1421" );
    arduinoComm.fd         = -1;
    arduinoComm.baudRate   = 9600;
    arduinoComm.timeOut    = 5000;
    arduinoComm.endOfLine  = '\n';
}
//------------------------------------------------------------------------------
int main( int argc, char* argv[] ){

//    float xi, float xi, float energiaTotal, float velocidadeCrescimento

    arvore_inicializar( 100, 100, 10, 1.0, 0.0, 1.0);

    arvore_simulaArvore();
//    arvore_imprime();

//     setupArduino();
//
//     int r, g, b;
//
//     printf( "R: " );
//     scanf( "%d", &r );
//
//     printf( "G: " );
//     scanf( "%d", &g );
//
//     printf( "B: " );
//     scanf( "%d", &b );
//
//     arduinoComm.buffer[ 0 ] = (char)r;
//     arduinoComm.buffer[ 1 ] = (char)g;
//     arduinoComm.buffer[ 2 ] = (char)b;
//     arduinoComm.buffer[ 3 ] = '\0';
//
// //    strcpy( arduinoComm.buffer, "zaz" );
//
//     arduino_LED( &arduinoComm );

    exit( EXIT_SUCCESS );
}
//------------------------------------------------------------------------------
