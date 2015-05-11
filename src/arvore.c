#include <allegro5/allegro.h>
// #include <allegro5/allegro_font.h>
// #include <allegro5/allegro_ttf.h>
// #include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//#include <allegro5/allegro.h>
//#include <allegro5/allegro_primitives.h>

#include "variaveis_arvore.h"
#include "random.h"

typedef struct galho {
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
    int   temFilhos;
    posicao_t posicao;
    bool_t crescer;


    struct galho *direita;
    struct galho *meio;
    struct galho *esquerda;
    struct galho *pai;

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
Galho *arvore_procuraGalhoPeloID( Galho *g, int id );
//------------------------------------------------------------------------------
int arvore_profundidadeGalho( Galho *g ){

    int contador = 0;
    Galho *galho = g;

    while ( galho != NULL ){
        galho = galho->pai;
        contador++;
    }

    return contador;
}
//------------------------------------------------------------------------------
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
void arvore_calculaProporcaoXY( Ponto *p ){

    p->x = randomFloat( 1.0, 3.0 );
    p->y = randomFloat( 1.0, 2.0 );

}
//------------------------------------------------------------------------------
void arvore_inicializar( float xi, float yi, int energiaTotal, float velocidadeCrescimento, float proporcaoX, float proporcaoY ){

    arvore.raiz         = NULL;//malloc( sizeof(Galho) );
    arvore.energiaTotal = energiaTotal;
    arvore.energiaAtivacao = 25;
    arvore.energiaLimite   = 30; // limite de crescimento em % da energia total
    arvore.id              = 0;
    arvore.velocidadeCrescimento = velocidadeCrescimento;
    arvore_adicionaGalho( energiaTotal, RAIZ, arvore.raiz, proporcaoX, proporcaoY, xi, yi );

    arvore.raiz->xi = xi;
    arvore.raiz->yi = yi;
    arvore.raiz->crescer = SIM;
    arvore.raiz->energiaRecebida = energiaTotal;

    arvore.raiz->temFilhos = NAO;

    srand( time(NULL) );

    return;

}
//------------------------------------------------------------------------------
void arvore_cresceGalho( Galho *g, int velocidadeCrescimento ){

    g->crescer = NAO;
    int profundidadeGalhos = arvore_profundidadeGalho( g );

    if ( g->energiaConsumida <= (g->energiaRecebida * (arvore.energiaLimite + profundidadeGalhos) / 100) && g->energiaRecebida > 0 ){
        switch ( g->posicao ) {
            case RAIZ:
                g->yf += ( g->proporcaoCrescimentoY * velocidadeCrescimento );
            break;
            case ESQUERDA:
                g->xf -= ( g->proporcaoCrescimentoX * velocidadeCrescimento );
                g->yf += ( g->proporcaoCrescimentoY * velocidadeCrescimento );
            break;
            case MEIO:
                g->yf += ( g->proporcaoCrescimentoY * velocidadeCrescimento );
            break;
            case DIREITA:
                g->xf += ( g->proporcaoCrescimentoX * velocidadeCrescimento );
                g->yf += ( g->proporcaoCrescimentoY * velocidadeCrescimento );
            break;
        }

        g->crescer = SIM;
    }

    return;
}
//------------------------------------------------------------------------------
void arvore_atualizaGalhos( Galho *g ){
//    printf("\nATUALIZA GALHOS: INICIO\n");
    if ( g == NULL)
        return;
return;
//    printf("\nATUALIZA GALHOS: 0\n");

    if ( g->pai != NULL){ // Não é a raiz
//        printf("\nATUALIZA GALHOS: pai != NULL\n");
        g->xi = g->pai->xf;
        g->yi = g->pai->yf;
    }

//    printf("\nATUALIZA GALHOS: 1\n");

    if ( g->crescer == SIM )
        g->energiaConsumida += arvore.velocidadeCrescimento;

    arvore_cresceGalho( g, arvore.velocidadeCrescimento );

//    printf("\nATUALIZA GALHOS: 2\n");
    // Adiciona novos galhos
    if ( g->crescer == NAO && g->energiaRecebida > 0 && g->temFilhos == NAO ){ // Galho cresceu tudo o que podia

        int quantosGalhos;
        int profundidadeGalhos = arvore_profundidadeGalho( g );

        if ( profundidadeGalhos <= 2 )
            quantosGalhos = 3;
        else
            quantosGalhos  = randomInt( 0, 3 );

//        printf("\nQUANTIDADE DE GALHOS A SEREM CRIADOS: %d\n", quantosGalhos);
        int primeiroGalho, segundoGalho, terceiroGalho;
        int energiaRecebida = g->energiaRecebida - g->energiaConsumida;
        int primeiroGalhoEnergia, segundoGalhoEnergia, terceiroGalhoEnergia; // transfere a energia excedente depois que o galho cresceu tudo que podia

        Ponto proporcao;

        arvore_calculaProporcaoXY( &proporcao );
        g->temFilhos = SIM;

//        printf("\nATUALIZA GALHOS: 3\n");
        // Força para que as primeiras ramificações da árvore sejam sempre 3

//        printf( "\nquantosGalhos: %d", quantosGalhos );
        switch ( quantosGalhos ){
            case 1:
                primeiroGalhoEnergia = g->energiaRecebida - g->energiaConsumida;
                primeiroGalho = randomInt( 0, 3 ) - 1; // 0 = direita,  1 = meio,  2 = esquerda
                if ( primeiroGalhoEnergia > 1 )
                    arvore_adicionaGalho( primeiroGalhoEnergia, primeiroGalho, g, proporcao.x, proporcao.y, g->xi, g->yi );
            break;
            case 2:
                primeiroGalho = randomInt( 0, 3 ) - 1; // 0 = direita,  1 = meio,  2 = esquerda
                segundoGalho = primeiroGalho; // Valor inicial para disparar o looping abaixo

                primeiroGalhoEnergia = energiaRecebida / randomInt( 0, 3 ) - 1;
                segundoGalhoEnergia  = energiaRecebida - primeiroGalhoEnergia;

                do { // repete enquanto o número é igual a 1
                    segundoGalho = randomInt( 0, 3 ) - 1;
                } while ( segundoGalho == primeiroGalho );

                if ( primeiroGalhoEnergia > 1 ){
                    arvore_calculaProporcaoXY( &proporcao );
                    arvore_adicionaGalho( primeiroGalhoEnergia, primeiroGalho, g, proporcao.x, proporcao.y, g->xi, g->yi );
                }

                if ( segundoGalhoEnergia > 1 ){
                    arvore_calculaProporcaoXY( &proporcao );
                    arvore_adicionaGalho( segundoGalhoEnergia, segundoGalho, g, proporcao.x, proporcao.y, g->xi, g->yi );
                }
            break;
            case 3: // cria três galhos
                primeiroGalho = randomInt( 0, 3 ) - 1; // 0 = direita,  1 = meio,  2 = esquerda
                segundoGalho = primeiroGalho; // Valor inicial para disparar o looping abaixo
                terceiroGalho = primeiroGalho; // Valor inicial para disparar o looping abaixo

                if ( profundidadeGalhos <= 2 ) { // Energia distribuida igualmente para as primeiras ramificações
                    primeiroGalhoEnergia = energiaRecebida / 3;
                    segundoGalhoEnergia  = primeiroGalhoEnergia;
                    terceiroGalhoEnergia = primeiroGalhoEnergia;
                }
                else {
                    primeiroGalhoEnergia = energiaRecebida / randomInt( 0, 3 ) - 1;
                    segundoGalhoEnergia  = energiaRecebida - primeiroGalhoEnergia;
                    terceiroGalhoEnergia = energiaRecebida - ( primeiroGalhoEnergia + segundoGalhoEnergia );
                }

                do { // segundo galho precisa ser diferente do primeiro galho
                    segundoGalho = randomInt( 0, 3 ) - 1;
                } while ( segundoGalho == primeiroGalho );

                int flag = 1;

                do { // terceiro galho precisa ser diferente dos primeiro e segundo galhos
                    terceiroGalho = randomInt( 0, 3 ) - 1;
                    if ( terceiroGalho != primeiroGalho && terceiroGalho != segundoGalho )
                        flag = 0;
//                    printf("\nlooping..... terceiro galho: %d\n", terceiroGalho );
                } while ( flag );

//                printf("\nCriando 3 galhos...\n");

                if ( primeiroGalhoEnergia > 1 ){
                    arvore_calculaProporcaoXY( &proporcao );
                    arvore_adicionaGalho( primeiroGalhoEnergia, primeiroGalho, g, proporcao.x, proporcao.y, g->xi, g->yi );
                }

                if ( segundoGalhoEnergia > 1 ){
                    arvore_calculaProporcaoXY( &proporcao );
                    arvore_adicionaGalho( segundoGalhoEnergia, segundoGalho, g, proporcao.x, proporcao.y, g->xi, g->yi );
                }

                if ( terceiroGalhoEnergia > 1 ){
                    arvore_calculaProporcaoXY( &proporcao );
                    arvore_adicionaGalho( terceiroGalhoEnergia, terceiroGalho, g, proporcao.x, proporcao.y, g->xi, g->yi );
                }
            break;
            // default:
            //     return;
        }
    }

    arvore_atualizaGalhos( g->esquerda );
    arvore_atualizaGalhos( g->meio );
    arvore_atualizaGalhos( g->direita );

//    printf("\nATUALIZA GALHOS: FIM\n");
    return;
}
//------------------------------------------------------------------------------
void arvore_simulaArvore( void ){
//    printf("\nSIMULA ARVORE\n");
    for ( int i = 0; i <= arvore.energiaTotal; i += 1 /*arvore.velocidadeCrescimento*/ ){
            arvore_atualizaGalhos( arvore.raiz );
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
    novoGalho->direita   = NULL;
    novoGalho->meio      = NULL;
    novoGalho->direita   = NULL;
    novoGalho->pai       = galhoPai;
    novoGalho->id        = arvore.id;
    novoGalho->posicao   = posicao;
    novoGalho->temFilhos = NAO;
    novoGalho->crescer   = SIM;

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

    printf( "**Galho criado. ID: %d\n", novoGalho->id );
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

    int quantidadeGalhos = 0;
    int quantidadeFilhos = 0;

    // int profundidadeEsquerda = 0;
    // int profundidadeMeio     = 0;
    // int profundidadeDireita  = 0;

    quantidadeGalhos += 1 + imprime( g->esquerda );
    quantidadeGalhos += 1 + imprime( g->meio );
    quantidadeGalhos += 1 + imprime( g->direita );

    printf( "\n----------------------------------------\n" );
    printf( "ID: %d\n", g->id );

    if ( g->pai != NULL )
        printf( "ID Pai: %d\n", g->pai->id );

    printf( "Nível: %d\n", arvore_profundidadeGalho(g) );

    if ( g->esquerda != NULL)
        quantidadeFilhos++;

    if ( g->meio != NULL)
        quantidadeFilhos++;

    if ( g->direita != NULL)
        quantidadeFilhos++;

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

    switch ( g->crescer ){
        case SIM:
            printf( "Crescer: SIM\n");
        break;
        default:
            printf( "Crescer: NAO\n" );
    }

    // printf( "\nProfundidade esquerda : %d\n", profundidadeEsquerda );
    // printf( "Profundidade meio     : %d\n", profundidadeMeio );
    // printf( "Profundidade direita  : %d\n", profundidadeDireita );
    printf( "Quantidade de filhos: %d\n", quantidadeFilhos );
    printf( "Energia consumida: %d\n", g->energiaConsumida );
    printf( "Energia recebida: %d\n", g->energiaRecebida );
    printf( "Xi: %.2f  Yi: %.2f \nXf: %.2f  Yf: %.2f\n", g->xi, g->yi, g->xf, g->yf );
    printf( "Proporção de crescimento X: %.2f\n", g->proporcaoCrescimentoX );
    printf( "Proporção de crescimento Y: %.2f\n", g->proporcaoCrescimentoY );
    printf( "----------------------------------------\n" );

    return quantidadeGalhos;
}
//------------------------------------------------------------------------------
void arvore_imprime( void ){
    printf("\nIMPRIME ARVORE\n");

    int quantidadeGalhos = imprime( arvore.raiz );

    printf( "\nQuantidade de galhos: %d\n", quantidadeGalhos );

    return;
}
//------------------------------------------------------------------------------
