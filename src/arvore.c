#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "variaveis_arvore.h"
#include "random.h"

typedef struct _galho {
    // Cálculo da posição Yf (y) da árvore:
    // Energia = hipotenusa (h)
    // Xf = cateto adjascente (x)
    // (h)^2 = (x)^2 + (y)^2  =>  (y)^2 = (h)^2 - (x)^2
    //
    // Ou: sen(a) = h/y, com a = ângulo de crescimento

    int energiaRecebida; // Energia passada (herdada) para o galho

    int energiaConsumida;    // Quanto da energia de crescimento total está sendo utilizada no momento.
                           // Tem relação direta com o tamanho do galho no momento.

    float xi, yi;          // Coordenadas onde o galho começa o crescimento.
                           // Essas mesmas coordenadas são IGUAIS às coordenadas
                           // do final do galho anterior.

    float xf, yf;          // Coordenadas do final do galho. Também define o início do próximo galho

    float proporcaoCrescimentoX;
    float proporcaoCrescimentoY;

    int   id;
    posicao_t posicao;
    bool_t crescer;


    struct _galho *direita;
    struct _galho *meio;
    struct _galho *esquerda;
    struct _galho *pai;

} Galho;


typedef struct {
    int energiaLimite;   // limite de energia a ser gasta ( em % ). Se a energia recebida for abaixo do valor especificado,
                           // então o galho para de crescer e passa a produzir folhas e frutos.
                           // Caso o valor seja maior, o excedente será distribuido para os galhos filhos

    int energiaAtivacao; // Calculada em %. Ativa o crescimento dos galhos filhos, mesmo que o galho não terminou de crescer.
                           // Isso possibilita diversos galhos crescerem ao mesmo tempo, dando aspecto mais natural.

    int energiaTotal; // Energia total de crescimento da árvore
    float velocidadeCrescimento;
    Galho *raiz;
    int id; // Contador para gerar id único para cada galho
} Arvore;

Arvore arvore;

int arvore_adicionaGalho( int, posicao_t, Galho*, float, float, float, float );
void arvore_imprime( void );
//------------------------------------------------------------------------------
// #ifndef _ARVORE_INICIALIZADA_
// #define _ARVORE_INICIALIZADA_
//
//     arvore.energiaTotal = 0;
//     arvore.raiz = NULL;
//
// #endif
//------------------------------------------------------------------------------
void arvore_inicializar( float xi, float yi, int energiaTotal, float velocidadeCrescimento, float proporcaoX, float proporcaoY ){

    arvore.raiz         = NULL;//malloc( sizeof(Galho) );
    arvore.energiaTotal = energiaTotal;
    arvore.energiaAtivacao = 25;
    arvore.energiaLimite   = 40;
    arvore.id              = 0;
    arvore.velocidadeCrescimento = velocidadeCrescimento;
    arvore_adicionaGalho( energiaTotal, RAIZ, arvore.raiz, proporcaoX, proporcaoY, xi, yi );

    arvore.raiz->xi = xi;
    arvore.raiz->yi = yi;
    arvore.raiz->crescer = SIM;
    arvore.raiz->energiaRecebida = energiaTotal;

    srand( time(NULL) );

    return;

}
//------------------------------------------------------------------------------
void arvore_cresceGalho( Galho *g, int velocidadeCrescimento ){

    g->crescer = NAO;

    if ( g->energiaConsumida < ((g->energiaRecebida * arvore.energiaLimite) / 100) ){
        g->xf += ( g->proporcaoCrescimentoX * velocidadeCrescimento );
        g->yf += ( g->proporcaoCrescimentoY * velocidadeCrescimento );
        g->crescer = SIM;
    }

    return;
}
//------------------------------------------------------------------------------
void arvore_atualizaGalhos( Galho *g ){

    if ( g == NULL)
        return;

    if ( g->pai != NULL){
        g->xi = g->pai->xf;
        g->yi = g->pai->yf;
    }

    // Pre-order
    if ( g->crescer )
        g->energiaConsumida += arvore.velocidadeCrescimento;

    arvore_cresceGalho( g, arvore.velocidadeCrescimento );


    arvore_atualizaGalhos( g->esquerda );
    arvore_atualizaGalhos( g->meio );
    arvore_atualizaGalhos( g->direita );

    // Post-order
    if ( g->crescer == NAO ){ // Galho cresceu tudo o que podia
        int quantosGalhos = randomInt( 1, 3 );
        //printf("\nQUANTIDADE DE GALHOS A SEREM CRIADOS: %d", quantosGalhos);

        switch ( quantosGalhos ){
            case 1:
                int primeiroGalho = randomInt( 0, 2 );
                arvore_adicionaGalho( transferirEnergia, primeiroGalho, g, proporcaoX, proporcaoY, g->xi, g->yi );
            break;
            case 2:
                int primeiroGalho = randomInt( 0, 2 );
                int segundoGalho;

                if ( primeiroGalho = 2 )
                    segundoGalho = randomInt( 0, 1 );
                else if ( primeiroGalho = 1 )
                    segundoGalho = randomInt( 4, 5 ) % 2; // direita =
            break;
            case 3:
            break;
        }

        int energiaE, energiaM, energiaD; // quantidade de energia repassada para a esquerda, meio e direita

        int transferirEnergia = g->energiaRecebida - g->energiaConsumida;

        float proporcaoX = randomFloat( 0.5, 1.2 );
        float proporcaoY = randomFloat( 0.5, 1.2 );

    }

    return;
}
//------------------------------------------------------------------------------
void arvore_simulaArvore(){

    for ( int i = 0; i <= arvore.energiaTotal; i += arvore.velocidadeCrescimento ){
        // if ( arvore.raiz->crescer ){
        //     arvore.raiz->energiaConsumida += arvore.velocidadeCrescimento;
        //     arvore_cresceGalho( arvore.raiz, arvore.velocidadeCrescimento );
            arvore_atualizaGalhos( arvore.raiz );
//            arvore_imprime();
//        }
    }

    arvore_imprime();
    return;
}
//------------------------------------------------------------------------------
int arvore_adicionaGalho( int energiaRecebida, posicao_t posicao,
                          Galho* galhoPai, float proporcaoX, float proporcaoY, float xi, float yi ){

    Galho *novoGalho = malloc( sizeof(Galho) );

    novoGalho->energiaRecebida       = energiaRecebida;
    novoGalho->proporcaoCrescimentoX = proporcaoX;
    novoGalho->proporcaoCrescimentoY = proporcaoY;
    novoGalho->energiaConsumida = 0;
    novoGalho->direita  = NULL;
    novoGalho->meio     = NULL;
    novoGalho->direita  = NULL;
    novoGalho->pai      = galhoPai;
    novoGalho->id       = arvore.id;
    novoGalho->posicao  = posicao;

    arvore.id += 1;

    if ( galhoPai == NULL ) {
        novoGalho->xi = xi; // ** Mudar para valores-padrão
        novoGalho->yi = yi; // ** Mudar para valores-padrão
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

    // printf( "\nProfundidade esquerda : %d\n", profundidadeEsquerda );
    // printf( "Profundidade meio     : %d\n", profundidadeMeio );
    // printf( "Profundidade direita  : %d\n", profundidadeDireita );
    printf( "Energia atual: %d\n", g->energiaConsumida );
    printf( "Energia de crescimento: %d\n", g->energiaRecebida );
    printf( "Xi: %.2f  Yi: %.2f \nXf: %.2f  Yf: %.2f\n", g->xi, g->yi, g->xf, g->yf );
    printf( "Proporção de crescimento X: %.2f\n", g->proporcaoCrescimentoX );
    printf( "Proporção de crescimento Y: %.2f\n", g->proporcaoCrescimentoY );
    printf( "----------------------------------------\n" );

    return 0;
}
//------------------------------------------------------------------------------
void arvore_imprime( void ){

    imprime( arvore.raiz );
    return;
}
//------------------------------------------------------------------------------
