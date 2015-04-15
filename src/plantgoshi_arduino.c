/* PlantGoshi - 2015
 * Centro Universitário Senac - Santo Amaro
 *
 * Authors: Anderson J. da Silva, Felipe R. de Luca, Nelson J. Dressler
 *
 *
 * The code of this library is based on the original demo program
 * made by Tod E. Kurt, http://todbot.com/blog/
 * All Arduino-related code is credited to him.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "arduino-serial-lib.h"

// Configuração
typedef struct {

    const int max_buff = 256;

    int baudRate = 9600;
    int timeOut  = 5000;
    int fd       = -1;
    int rc, n;

    char serialPort[ max_buff ]; // Informar o caminho completo da porta serial no Linux ou Mac OSX
    char buffer[ max_buff ];
    char endOfLine = '\n';

} ArduinoComm;

//------------------------------------------------------------------------------
// Envia comando para mudar a cor do LED RGB
void arduino_LED( ArduinoComm* comm, float R, float G, float B ){

    if( comm->fd != -1 ) // A porta serial já está aberta
        serialport_close( fd );

    strcpy( comm->serialPort,  );
    comm->fd = seriaport_init( comm->serialPort, comm->baudRate );

    if ( comm->fd == -1 ){
        error( "Couldn't open port" );
        return;
    }

    comm->rc = serialport_write( comm->fd, comm->buffer );
        if( rc == -1 ) error( "error writing to Arduino" );

    serialport_flush( fd );

    return;
}
//------------------------------------------------------------------------------
