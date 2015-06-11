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

#include "arduino-serial-lib.h"
#include "plantgoshi_arduino.h"


//------------------------------------------------------------------------------
// Envia comando para mudar a cor do LED RGB
void arduino_LED( ArduinoComm* comm ){

    // printf( "Buffer: %s\n", comm->buffer );
    // printf( "Serilport: %s\n", comm->serialPort );
    // printf( "Comm: %d\n", comm->fd );

    if( comm->fd != -1 ) // A porta serial já está aberta
        serialport_close( comm->fd );

    comm->fd = serialport_init( comm->serialPort, comm->baudRate );

    if ( comm->fd == -1 ){
        //    printf( "Couldn't open port\n" );
        return;

    }

    serialport_flush( comm->fd );

    comm->rc = serialport_write( comm->fd, comm->buffer );

    if( comm->rc == -1 )
        printf( "error writing to Arduino\n" );



    // serialport_close( comm->fd );


    return;
}
//------------------------------------------------------------------------------
