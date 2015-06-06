#ifndef __IMAGE_PROC_H__
#define __IMAGE_PROC_H__

#include <allegro5/allegro.h>
#include "cor.h"

void imageProc_init( void );
void imageProc_finaliza( void );
void imageProc_atualizaXY( int *x, int *y, FaixaCor_t c );
void imageProc_desenhaImagem( ALLEGRO_BITMAP *telaEsquerda, ALLEGRO_BITMAP *telaDireita );

int imageProc_getLargura( void );
int imageProc_getAltura( void );

#endif
