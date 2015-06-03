#ifndef __VARIAVEIS_ARVORE_H__
#define __VARIAVEIS_ARVORE_H__

typedef enum _posicaoGalho {
    ESQUERDA = 0,
    MEIO     = 1,
    DIREITA  = 2,
    RAIZ     = 3
} posicao_t;

typedef enum {
    NAO = 0,
    SIM = 1
} bool_t;

typedef struct {
    float x;
    float y;
    int id;
} Ponto;

#endif
