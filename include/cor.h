#ifndef __COR_H__
#define __COR_H__

typedef enum{
	VERMELHO = 0,
	AMARELO  = 1,
	VERDE    = 2,
	CIANO    = 3,
	AZUL     = 4,
	MAGENTA  = 5,

	BRANCO   = 6,
	PRETO    = -1,
} FaixaCor_t;

typedef struct {
    float h_a1, h_a2, h_b1, h_b2; // ângulo da matiz
    float minS, minV; // saturação e valor mínimo
    unsigned char minLuma; // luminancia mínima
} Cor;

#endif
