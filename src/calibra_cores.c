#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>

#include "image_proc.h"
#include "math.h"
#include "cor.h"

#define FPS 60

FaixaCor_t FaixaCor; // Enum criado pelo Nelson

void erro(char *mensagem) {
  fprintf(stderr, "%s\n", mensagem);

  exit(EXIT_FAILURE);
}

///-----------------------------------------------------------------------------
void opcoesInicializacao( void ){

    printf("PlantGoshi v. 1.0\n");
    printf("=========================================\n");
    printf("Digite o número da cor a ser utilizada no rastreamento:\n\n");
    printf("1 - amarelo\n");
    printf("2 - verde\n");
    printf("3 - ciano\n");
    printf("4 - azul\n");
    printf("5 - magenta\n");
    printf("6 - vermelho\n\n");

    exit(EXIT_FAILURE);
}
//-------------------------------------------------------------------------------
int main( int argc, char **argv ) {

    unsigned char opt = '7';

    if ( argc < 2 ){
        opcoesInicializacao();
    }
    else{
        opt = argv[1][0];

        if ( !(opt == '1' || opt == '2' || opt == '3' || opt == '4' || opt == '5' || opt == '6') )
            opcoesInicializacao();
    }

  imageProc_init( 1 ); // Função de inicialização

  if(!al_init())
    erro("nao foi possivel inicializar allegro");

  if(!al_init_primitives_addon())
    erro("nao foi possivel inicializar adicional de primitivas");

  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  if(!queue)
    erro("nao foi possivel criar fila de eventos");

  ALLEGRO_DISPLAY *display = al_create_display( 2 * imageProc_getLargura(), imageProc_getAltura() );
  if(!display)
    erro("nao foi possivel criar janela");

  al_register_event_source(queue, al_get_display_event_source(display));

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  if(!timer)
    erro("nao foi possivel criar relogio");

  al_register_event_source(queue, al_get_timer_event_source(timer));

  /**********/

  ALLEGRO_COLOR al_colorGreen = al_map_rgb_f(0.2, 1.0, 0.2);
  ALLEGRO_COLOR al_colorRed = al_map_rgb_f(1.0, 0.2, 0.2);

  ALLEGRO_BITMAP *buffer = al_get_backbuffer(display);

  ALLEGRO_BITMAP *esquerda = al_create_sub_bitmap( buffer, 0, 0, imageProc_getLargura(), imageProc_getAltura() );

  ALLEGRO_BITMAP *direita = al_create_sub_bitmap( buffer, imageProc_getLargura(), 0, imageProc_getLargura(), imageProc_getAltura() );

  /**********/

  int continuar = 1;
  int atualizar = 0;

  al_start_timer(timer);

  al_rest(1);

  printf("===========================================\n");
  printf("iniciando calibragem de cores\n");
  printf("===========================================\n");

 // int calibrarCor = 0;
//  int corCalibrada = 0;
  FaixaCor_t faixaCor = PRETO;
//  printf("%c\n", opt );

  printf("-------------------------------------------\n");
  if ( opt == '6' ){
      faixaCor = VERMELHO;
      printf("Calibrando VERMELHO..\n");
  }
  if ( opt == '1' ){
      faixaCor = AMARELO;
      printf("Calibrando AMARELO\n");
  }
  if ( opt == '2' ){
      faixaCor = VERDE;
      printf("Calibrando VERDE\n");
  }
  if ( opt == '3' ){
      faixaCor = CIANO;
      printf("Calibrando CIANO\n");
  }
  if ( opt == '4' ){
      faixaCor = AZUL;
      printf("Calibrando AZUL\n");
  }
  if ( opt == '5' ){
      faixaCor = MAGENTA;
      printf("Calibrando MAGENTA\n");
  }

  printf("-------------------------------------------\n");

 // imageProc_carregaCores();

  while(continuar) {
      ALLEGRO_EVENT event;

      al_wait_for_event(queue, &event);

      switch(event.type) {
          case ALLEGRO_EVENT_DISPLAY_CLOSE:
          continuar = 0;
      break;
          case ALLEGRO_EVENT_TIMER:
          atualizar = 1;
      break;
      }

      if ( atualizar && al_is_event_queue_empty(queue) ) {
          int x = 0, y = 0; // temporario

          imageProc_atualizaXY( &x, &y, faixaCor ); // Função principal
//          imageProc_calibraCor( faixaCor, 80, 200, 80, 180 );

          imageProc_desenhaImagem( esquerda, direita ); // Mostra o processamento. Função opcional.

          al_draw_circle( x, y, 10, al_colorGreen, 3 );

          al_draw_rectangle( 80, 80, 200, 180, al_colorRed, 5 );

          al_flip_display();

          atualizar = 0;
      }
  }

  al_stop_timer(timer);

  /**********/

  al_destroy_bitmap(direita);

  al_destroy_bitmap(esquerda);

  imageProc_finaliza(); // Libera todas as matrizes criadas

  /**********/

  al_unregister_event_source(queue, al_get_timer_event_source(timer));
  al_destroy_timer(timer);

  al_unregister_event_source(queue, al_get_display_event_source(display));
  al_destroy_display(display);

  al_destroy_event_queue(queue);
  al_shutdown_primitives_addon();
  al_uninstall_system();

  return EXIT_SUCCESS;
 }
