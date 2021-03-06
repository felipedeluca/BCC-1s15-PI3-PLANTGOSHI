#ifndef __ARVORE_H__
#define __ARVORE_H__

#include <allegro5/allegro.h>

void arvore_imprime( void );
void arvore_inicializar( float xi, float yi, int energiaTotal, float velocidadeCrescimento,
     float proporcaoX, float proporcaoY, int offsetX, int offsetY );
void arvore_simulaArvore( int energiaExtra );
void arvore_desenha( ALLEGRO_BITMAP *bmp );
void arvore_proximoPontoCrescimentoFrutas( int *x, int *y, int *id );
void arvore_atualizaXYGalho( int *x, int *y, int id );
void arvore_destroi( void );

#endif
