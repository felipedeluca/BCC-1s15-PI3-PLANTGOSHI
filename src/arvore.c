#include <stdlib.h>
#include <stdio.h>

#include "variaveis_arvore.h"

typedef struct _galho {
    float energiaRecebida; // Energia passada (herdada) para o galho
    float energiaLimite;   // limite de energia a ser gasta. Se a energia recebida for abaixo do valor especificado,
                           // então o galho para de crescer e passa a produzir folhas e frutos.
                           // Caso o valor seja maior, o excedente será distribuido para os galhos filhos

    float xi, yi;          // Coordenadas onde o galho começa o crescimento.
                           // Essas mesmas coordenadas são IGUAIS às coordenadas
                           // do final do galho anterior.

    float xf, yf;          // Coordenadas do final do galho. Também define o início do próximo galho

    float velocidadeCrescimento;
    float anguloCrescimento;
    int   id;
    posicao_t posicao;


    struct _galho *direita;
    struct _galho *meio;
    struct _galho *esquerda;
    struct _galho *pai;
} Galho;


typedef struct {
    float energiaTotal; // Energia total de crescimento da árvore
    Galho *raiz;
    int id; // Contador para gerar id único para cada galho
} Arvore;

Arvore arvore;

int arvore_adicionaGalho( float, float, posicao_t, float, float, Galho* );

//------------------------------------------------------------------------------
// #ifndef _ARVORE_INICIALIZADA_
// #define _ARVORE_INICIALIZADA_
//
//     arvore.energiaTotal = 0;
//     arvore.raiz = NULL;
//
// #endif
//------------------------------------------------------------------------------
void arvore_inicializar( float energiaTotal ){

    arvore.raiz         = NULL;//malloc( sizeof(Galho) );
    arvore.energiaTotal = energiaTotal;
    arvore.id           = 0;
    arvore_adicionaGalho( 100.0, 50.0, RAIZ, 90.0, 1.0, arvore.raiz );

}
//------------------------------------------------------------------------------
int arvore_adicionaGalho( float energiaRecebida, float energiaLimite, posicao_t posicao,
                           float anguloCrescimento, float velocidadeCrescimento, Galho* galhoPai ){

    Galho *novoGalho = malloc( sizeof(Galho) );

    novoGalho->velocidadeCrescimento = velocidadeCrescimento;
    novoGalho->energiaRecebida   = energiaRecebida;
    novoGalho->energiaLimite     = energiaLimite;
    novoGalho->anguloCrescimento = anguloCrescimento;
    novoGalho->direita  = NULL;
    novoGalho->meio     = NULL;
    novoGalho->direita  = NULL;
    novoGalho->pai      = galhoPai;
    novoGalho->id       = arvore.id;
    novoGalho->posicao  = posicao;

    arvore.id += 1;

    if ( galhoPai == NULL ) {
        novoGalho->xi = 100; // ** Mudar para valores-padrão
        novoGalho->yi = 100; // ** Mudar para valores-padrão
        novoGalho->xf = novoGalho->xi;
        novoGalho->yf = novoGalho->yi;
        arvore.raiz = novoGalho;
        //printf( "RAIZ | Angulo: %.2f\n", novoGalho->anguloCrescimento );
        return novoGalho->id;
    }

    novoGalho->xi = galhoPai->xf;
    novoGalho->yi = galhoPai->yf;

    novoGalho->xf = novoGalho->xi;
    novoGalho->yf = novoGalho->yi;

    switch ( posicao ){
        case RAIZ:
            // Nao faz nada. A raiz já foi incluída
        break;
        case DIREITA:
            galhoPai->direita = novoGalho;
        break;
        case MEIO:
            galhoPai->meio = novoGalho;
        break;
        case ESQUERDA:
            galhoPai->esquerda = novoGalho;
        break;
    }

    return novoGalho->id;
}
//------------------------------------------------------------------------------
Galho *arvore_procuraGalhoPeloID( Galho *g, int id ){

    if ( g == NULL || g->id == id )
        return g;

    Galho *galho, *temp;
    galho = NULL;

    temp = arvore_procuraGalhoPeloID( g->esquerda, id );
    if ( temp != NULL )
        galho = temp;

    temp = arvore_procuraGalhoPeloID( g->meio, id );
    if ( temp != NULL )
        galho = temp;

    temp = arvore_procuraGalhoPeloID( g->direita, id );
    if ( temp != NULL )
        galho = temp;

    return galho;
}
//------------------------------------------------------------------------------
int imprime( Galho *g ){
    // Imprime em pós-ordem

    if ( g == NULL )
        return -1;

    int profundidadeEsquerda = 0;
    int profundidadeMeio     = 0;
    int profundidadeDireita  = 0;

    profundidadeEsquerda += 1 + imprime( g->esquerda );
    profundidadeMeio     += 1 + imprime( g->meio );
    profundidadeDireita  += 1 + imprime( g->direita );

    printf( "\n----------------------------------------\n" );
    printf( "ID: %d\n", g->id );

    switch ( g->posicao ){
        case RAIZ:
            printf( "Posicao: RAIZ\n" );
        break;
        case ESQUERDA:
            printf( "Posicao: ESQUERDA\n" );
        break;
        case MEIO:
            printf( "Posicao: MEIO\n" );
        break;
        case DIREITA:
            printf( "Posicao: DIREITA\n" );
        break;
    }

    printf( "\nProfundidade esquerda : %d\n", profundidadeEsquerda );
    printf( "Profundidade meio     : %d\n", profundidadeMeio );
    printf( "Profundidade direita  : %d\n", profundidadeDireita );
    printf( "Energia de crescimento: %.2f\n", g->energiaRecebida );
    printf( "Energia limite: %.2f\n", g->energiaLimite );
    printf( "Xi: %.2f  Yi: %.2f\nXf: %.2f  Yf: %.2f\n", g->xi, g->yi, g->xf, g->yf );
    printf( "Angulo de crescimento: %.2f\n", g->anguloCrescimento );
    printf( "----------------------------------------\n" );

    return 0;
}
//------------------------------------------------------------------------------
void arvore_imprime( void ){
    //TESTES SIMULANDO A ARVORE CRESCENDO
    arvore.raiz->xf = 110;
    arvore.raiz->yf = 230;

    arvore_adicionaGalho( 100.0, 50.0, DIREITA, 10.0, 1.0, arvore.raiz );
    int id = arvore.id; // O id vai incrementando a cada adição de um novo galho

    arvore_adicionaGalho( 50.0, 10.0, ESQUERDA, 270.0, 1.0, arvore.raiz );
    id = arvore.id;

    arvore_adicionaGalho( 40.0, 5.0, ESQUERDA, 350.0, 1.0, arvore_procuraGalhoPeloID( arvore.raiz, id - 1 ) );
    id = arvore.id;

    arvore_adicionaGalho( 35.0, 5.0, MEIO, 0.0, 1.0, arvore_procuraGalhoPeloID( arvore.raiz, id - 1 ) );
    arvore_adicionaGalho( 30.0, 20.0, MEIO, 12.0, 1.0, arvore.raiz );

    imprime( arvore.raiz );
}
//------------------------------------------------------------------------------
