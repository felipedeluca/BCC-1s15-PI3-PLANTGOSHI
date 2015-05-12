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

    arvore_inicializar( 0, 0, 1100, 1.0, 0.0, 1.5, 600, 700);

    int largura = 1280;
    int altura = 720;
    // //
    //
    if(!al_init())
      erro("nao foi possivel inicializar allegro");

     ALLEGRO_DISPLAY *display = NULL;
     display = al_create_display(largura, altura);
    // //
    if(!display)
        erro("nao foi possivel criar janela");

    if(!al_init_primitives_addon())
      erro("nao foi possivel inicializar adicional de primitivas");

    // ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    // if(!queue)
    //   erro("nao foi possivel criar fila de eventos");


    // al_register_event_source(queue, al_get_display_event_source(display));

    // ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    // if(!timer)
    //   erro("nao foi possivel criar relogio");


   ALLEGRO_BITMAP *buffer = al_get_backbuffer(display);
//
    al_set_target_bitmap( buffer );
    al_clear_to_color(al_map_rgb(255,255,255));

    arvore_simulaArvore();
    arvore_desenha( buffer );
// //    al_register_event_source(queue, al_get_timer_event_source(timer));
//
// printf( "al_flip_display()\n" );

//    al_set_target_bitmap( buffer );
    al_flip_display();

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
    al_rest( 10.0 );
    //
    al_destroy_display(display);
// //    al_destroy_bitmap(buffer);
//
// //    al_shutdown_primitives_addon();
//     al_uninstall_system();
    printf( "\nFIM\n" );
    exit( EXIT_SUCCESS );
}
//------------------------------------------------------------------------------
