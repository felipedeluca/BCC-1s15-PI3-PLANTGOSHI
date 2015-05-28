#include <allegro5/allegro.h>
// #include <allegro5/allegro_font.h>
// #include <allegro5/allegro_ttf.h>
// #include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
// #include <allegro5/allegro_audio.h>
// #include <allegro5/allegro_acodec.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "plantgoshi_arduino.h"
#include "arvore.h"

#define FPS 60

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
void erro(char *mensagem) {
  fprintf(stderr, "%s\n", mensagem);

  exit(EXIT_FAILURE);
}
//------------------------------------------------------------------------------
int main( int argc, char **argv ){

    if(!al_init())
      erro("nao foi possivel inicializar allegro");

    int largura = 1280;
    int altura = 720;
    // //
    //

     ALLEGRO_DISPLAY *display = NULL;
     display = al_create_display(largura, altura);
    // //
    if(!display)
        erro("nao foi possivel criar janela");

    if(!al_init_primitives_addon())
      erro("nao foi possivel inicializar adicional de primitivas");

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if(!event_queue)
      erro("nao foi possivel criar fila de eventos");

    al_register_event_source(event_queue, al_get_display_event_source(display));

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    if(!timer)
      erro("nao foi possivel criar relogio");


   ALLEGRO_BITMAP *bitmap = al_get_backbuffer( display );
//
    al_set_target_bitmap( bitmap );

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
//
// printf( "al_flip_display()\n" );

//    al_set_target_bitmap( buffer );
/* POSIÇÃO ONDE CRESCER OS FRUTOS */
//    int x1, y1, id;

//    arvore_inicializar( 0, 0, 1700, 1.0, 0.0, 1.5, 600, 600 );
    arvore_inicializar( 0, 0, 1200, 1.0, 0.0, 1.5, 600, 600 );

    int contadorEnergia = 0;
    int energiaExtra = 0;

    int fpsCounter = 0;

    while( 1 ){

        ALLEGRO_EVENT event;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout( &timeout, 0.06 );

        bool get_event = al_wait_for_event_until( event_queue, &event, &timeout );

        if ( get_event && event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
            break;
        }


        contadorEnergia++;

        if ( contadorEnergia % 10 == 0 )
            energiaExtra = 1;

//        if ( fpsCounter % 2 == 0 ){
            al_clear_to_color(al_map_rgb(255,255,255));
            arvore_simulaArvore( energiaExtra );
            arvore_desenha( bitmap );
            energiaExtra = 0;

            al_flip_display();
//        }

        fpsCounter++;
        /* Retorna os pontos onde os frutos podem crescer */
//        arvore_proximoPontoCrescimentoFrutas( &x1, &y1, &id );
        //printf( "Proximo ponto x: %d  y: %d  id: %d\n", x1, y1, id );


    }


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
// printf( "al_rest( 2.0 )\n" );


    al_rest( 0.1 );
    //
    al_destroy_display( display );
    al_destroy_event_queue( event_queue );
// //    al_destroy_bitmap(buffer);
//
// //    al_shutdown_primitives_addon();
//     al_uninstall_system();
    printf( "\nFIM\n" );
    exit( EXIT_SUCCESS );
}
//------------------------------------------------------------------------------
