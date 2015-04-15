#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "plantgoshi_arduino.h"

ArduinoComm arduinoComm;
//------------------------------------------------------------------------------
int main( int argc, char* argv[] ){

    // Configura a comunicação com o Arduino
    strcpy( arduinoComm.serialPort, "/dev/cu.usbmodem1411" );
    strcpy( arduinoComm.buffer, "abracadabra!" );
    arduinoComm.fd         = -1;
    arduinoComm.baudRate   = 9600;
    arduinoComm.timeOut    = 5000;
    arduinoComm.endOfLine  = '\n';

    arduino_LED( &arduinoComm );

    exit( EXIT_SUCCESS );
}
//------------------------------------------------------------------------------
