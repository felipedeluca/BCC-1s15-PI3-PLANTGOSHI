#ifndef __COR_H__
#define __COR_H__

typedef enum{
	AMARELO  = 1,
	VERDE    = 2,
	CIANO    = 3,
	AZUL     = 4,
	MAGENTA  = 5,
	VERMELHO = 6,

	BRANCO   = 7,
	PRETO    = -1,
} FaixaCor_t;

typedef struct {
	FaixaCor_t tipo;
    float h_a1, h_a2, h_b1, h_b2; // ângulo da matiz
    float minS, minV; // saturação e valor mínimo
	float maxS, maxV; // saturação e valor mínimo
	int calibrada; // a cor já foi calibrada
	int numTentativas; // tentativas de calibragem
	int maxTentativas; // limite de tentativas de calibragem
	int posterizeRatio; // Posterizacao de cores
    float minLuma, maxLuma; // luminancia mínima
} Cor;

#endif
