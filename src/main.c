#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "plantgoshi_arduino.h"

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

    setupArduino();

    strcpy( arduinoComm.buffer, "zaz" );

    arduino_LED( &arduinoComm );

    exit( EXIT_SUCCESS );
}
//------------------------------------------------------------------------------
