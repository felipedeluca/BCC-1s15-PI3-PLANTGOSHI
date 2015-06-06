#include <allegro5/allegro.h>

#include "camera.h"
#include "estrutura_cam.h"
#include "cor.h"

#define POSTERIZE 100

unsigned char ***matrizFrame1;
unsigned char ***matrizFrame2;
unsigned char ***matrizFrame3;
unsigned char ***matrizFrame4;
unsigned char ***matrizFrame5;
unsigned char ***matrizFrame6;
unsigned char ***matriz;
unsigned char ***matrizRGB;
unsigned char ***matrizProcessada;
unsigned char ***matrizMediana;

camera *cam;
Cor vermelho, azul, verde, amarelo, laranja, ciano, magenta;

int frame;
//------------------------------------------------------------------------------
void imageProc_init( void ){

    cam = camera_inicializa(0);
    if(!cam){
        fprintf(stderr, "%s\n", "nao foi possivel inicializar camera");
        exit(EXIT_FAILURE);
    }

    matrizFrame1  = camera_aloca_matriz(cam);
    matrizFrame2  = camera_aloca_matriz(cam);
    matrizFrame3  = camera_aloca_matriz(cam);
    matrizFrame4  = camera_aloca_matriz(cam);
    matrizFrame5  = camera_aloca_matriz(cam);
    matrizFrame6  = camera_aloca_matriz(cam);
    matriz        = camera_aloca_matriz(cam);
    matrizRGB     = camera_aloca_matriz(cam);
    matrizMediana = camera_aloca_matriz(cam);
    matrizProcessada = camera_aloca_matriz(cam);

    vermelho.h_a1 = 0;
    vermelho.h_a2 = 30;
    vermelho.h_b1 = 340;
    vermelho.h_b2 = 360;
    vermelho.minS = 160;
    vermelho.minV = 0;
    vermelho.minLuma = 30;

    azul.h_a1 = 230;
    azul.h_a2 = 240;
    azul.h_b1 = 230;
    azul.h_b2 = 240;
    azul.minS = 30;
    azul.minV = 0;
    azul.minLuma = 30;

    verde.h_a1 = 90;
    verde.h_a2 = 150;
    verde.h_b1 = 90;
    verde.h_b2 = 150;
    verde.minS = 0;
    verde.minV = 0;
    verde.minLuma = 0;

    amarelo.h_a1 = 55;
    amarelo.h_a2 = 65;
    amarelo.h_b1 = 55;
    amarelo.h_b2 = 65;
    amarelo.minS = 150;
    amarelo.minV = 0;
    amarelo.minLuma = 50;

    frame = 1;
}
//------------------------------------------------------------------------------
int imageProc_getLargura( void ){
    return cam->largura;
}
//------------------------------------------------------------------------------
int imageProc_getAltura( void ){
    return cam->altura;
}
//------------------------------------------------------------------------------
void median( unsigned char ***m, unsigned char ***output, int y, int x, int altura, int largura ){
    // tira a média dos pixels em volta e salva o valor na coordenada x e y do pixel
    int sumR = 0;
    int sumG = 0;
    int sumB = 0;

    for ( int i = y - 3; i <= y + 3; i++ ){
        for ( int j = x - 3; j <= x + 3; j++ ){
//            printf("I: %d   J: %d\n", i, j);
            if ( i > 0 && j > 0 && i < altura && j < largura ){
                sumR += m[i][j][0];
                sumG += m[i][j][1];
                sumB += m[i][j][2];
            }
        }
    }

//    printf("x: %d   y: %d\n", x, y);
    output[y][x][0] = sumR / 49;
    output[y][x][1] = sumG / 49;
    output[y][x][2] = sumB / 49;
}
//------------------------------------------------------------------------------
unsigned char erode( unsigned char ***m, int y, int x, int altura, int largura ){
	int left = 0, right = 0, top = 0, bottom = 0, topLeft = 0, bottomLeft = 0, topRight = 0, bottomRight = 0;
	//printf("Entrei %d, %d\n", i, j);

    int x1 = x - 1;
    int x2 = x + 1;
    int y1 = y - 1;
    int y2 = y + 1;

	if ( y1 < 0 || m[y1][x][0] > 0 ){
        top = 1;
    }
	if ( y2 >= altura || m[y2][x][0] > 0 ){
        bottom = 1;
    }
	if ( x1 < 0 || m[y][x1][0] > 0 ){
        left = 1;
    }
	if ( x2 >= largura || m[y][x2][0] > 0 ){
        right = 1;
    }
    if ( (y1 < 0 || m[y1][x][0] > 0) & (x1 < 0 || m[y][x1][0] > 0) ) {
        topLeft = 1;
    }
    if ( (y1 < 0 || m[y1][x][0] > 0) & (x2 >= largura || m[y][x2][0] > 0) ) {
        topRight = 1;
    }
    if ( (y2 >= altura || m[y2][x][0] > 0) & (x1 < 0 || m[y][x1][0] > 0) ) {
        bottomLeft = 1;
    }
    if ( (y2 >= altura || m[y2][x][0] > 0) & (x2 >= largura || m[y][x2][0] > 0) ) {
        bottomRight = 1;
    }

	if( left & right & top & bottom & topLeft & bottomLeft & topRight & bottomRight ){
        return 255;
    }
	else{
        return 0;
    }

}
//------------------------------------------------------------------------------
void RGBtoHSV( unsigned char *r, unsigned char *g, unsigned char *b, float *h, float *s, float *v ) {

     float min = 0.0;
     float max = 0.0;
     float delta = 0.0;

     float _r = (*r * 1.00 / 255);
     float _g = (*g * 1.00 / 255);
     float _b = (*b * 1.00 / 255);

     min = MIN( _r, _g );
     min = MIN( min, _b );

     max = MAX( _r, _g );
     max = MAX( _b, max );

     delta = max - min;

     *v = max;				// v

     if( max != 0 ){
        // printf( "S: %.3f\n", *s );
          *s = delta / max;		// s
     }
     else {
         // r = g = b = 0		// s = 0, v is undefined
         *s = 0;
         *h = -1;
         return;
     }


     if( _r == max )
         *h = ( _g - _b ) / delta;		// between yellow & magenta
     else if( _g == max )
         *h = 2 + ( _b - _r ) / delta;	// between cyan & yellow
     else
         *h = 4 + ( _r - _g ) / delta;	// between magenta & cyan
     *h *= 60;				// degrees
     if( *h < 0 )
         *h += 360;
}
//------------------------------------------------------------------------------
unsigned char RGBtoY( unsigned char *r, unsigned char *g, unsigned char *b ) {
    return ( (0.299 * *r) + (0.587 * *g) + (0.114 * *b) );
}
//------------------------------------------------------------------------------
void processaMatriz( const unsigned char valorPixel, int y, int x ){

    // Buffer de 6 frames

    if ( frame % 6 == 1 ){
        matrizFrame1[y][x][0] = valorPixel;
        matrizFrame1[y][x][1] = valorPixel;
        matrizFrame1[y][x][2] = valorPixel;
    }
    else if ( frame % 6 == 2 ){
        matrizFrame2[y][x][0] = valorPixel;
        matrizFrame2[y][x][1] = valorPixel;
        matrizFrame2[y][x][2] = valorPixel;
    }
    else if ( frame % 6 == 3 ){
        matrizFrame3[y][x][0] = valorPixel;
        matrizFrame3[y][x][1] = valorPixel;
        matrizFrame3[y][x][2] = valorPixel;
    }
    if ( frame % 6 == 4 ){
        matrizFrame4[y][x][0] = valorPixel;
        matrizFrame4[y][x][1] = valorPixel;
        matrizFrame4[y][x][2] = valorPixel;
    }
    else if ( frame % 6 == 5 ){
        matrizFrame5[y][x][0] = valorPixel;
        matrizFrame5[y][x][1] = valorPixel;
        matrizFrame5[y][x][2] = valorPixel;
    }
    else if ( frame % 6 == 0 ){
        matrizFrame6[y][x][0] = valorPixel;
        matrizFrame6[y][x][1] = valorPixel;
        matrizFrame6[y][x][2] = valorPixel;
    }

    // buffer triplo
    matriz[y][x][0] = ( matrizFrame1[y][x][0] | matrizFrame2[y][x][0] | matrizFrame3[y][x][0] | matrizFrame4[y][x][0] | matrizFrame5[y][x][0] | matrizFrame6[y][x][0] );
    matriz[y][x][1] = ( matrizFrame1[y][x][1] | matrizFrame2[y][x][1] | matrizFrame3[y][x][1] | matrizFrame4[y][x][1] | matrizFrame5[y][x][1] | matrizFrame6[y][x][1] );
    matriz[y][x][2] = ( matrizFrame1[y][x][2] | matrizFrame2[y][x][2] | matrizFrame3[y][x][2] | matrizFrame4[y][x][2] | matrizFrame5[y][x][2] | matrizFrame6[y][x][2] );

    // median( matriz, matrizPreProcessada, y, x, cam->altura, cam->largura );

    unsigned char e = erode( matriz, y, x, cam->altura, cam->largura );
    matrizProcessada[y][x][0] = e;
    matrizProcessada[y][x][1] = e;
    matrizProcessada[y][x][2] = e;
}
//------------------------------------------------------------------------------
void imageProc_desenhaImagem( ALLEGRO_BITMAP *telaEsquerda, ALLEGRO_BITMAP *telaDireita ){

    camera_copia(cam, matrizProcessada, telaEsquerda);
    camera_copia(cam, matrizRGB, telaDireita);

}
//------------------------------------------------------------------------------
void processaImagem( FaixaCor_t cor ){

    camera_atualiza( cam );

    unsigned char mR; // Mediana vermelho
    unsigned char mG; // Mediana verde
    unsigned char mB; // Mediana azul

    unsigned char pR; // vermelho posterizado
    unsigned char pG; // verde posterizado
    unsigned char pB; // azul posterizado

    unsigned char luma;//

    float mH = 0; // mediana matriz
    float mS = 0; // mediana saturação
    float mV = 0; // mediana valor
    float pH = 0; // posterização matiz
    float pS = 0; // posterização saturação
    float pV = 0; // posterização valor

    unsigned char valorPixel = 0;

    Cor _cor;

    switch ( cor ){
        case VERMELHO:
            _cor.h_a1 = vermelho.h_a1;
            _cor.h_a2 = vermelho.h_a2;
            _cor.h_b1 = vermelho.h_b1;
            _cor.h_b2 = vermelho.h_b2;
            _cor.minS = vermelho.minS;
            _cor.minV = vermelho.minV;
            _cor.minLuma = vermelho.minLuma;
        break;
        case VERDE:
            _cor.h_a1 = verde.h_a1;
            _cor.h_a2 = verde.h_a2;
            _cor.h_b1 = verde.h_b1;
            _cor.h_b2 = verde.h_b2;
            _cor.minS = verde.minS;
            _cor.minV = verde.minV;
            _cor.minLuma = verde.minLuma;
        break;
        case AZUL:
            _cor.h_a1 = azul.h_a1;
            _cor.h_a2 = azul.h_a2;
            _cor.h_b1 = azul.h_b1;
            _cor.h_b2 = azul.h_b2;
            _cor.minS = azul.minS;
            _cor.minV = azul.minV;
            _cor.minLuma = azul.minLuma;
        break;
        case AMARELO:
            _cor.h_a1 = amarelo.h_a1;
            _cor.h_a2 = amarelo.h_a2;
            _cor.h_b1 = amarelo.h_b1;
            _cor.h_b2 = amarelo.h_b2;
            _cor.minS = amarelo.minS;
            _cor.minV = amarelo.minV;
            _cor.minLuma = amarelo.minLuma;
        break;
        default:
            _cor.h_a1 = 0;
            _cor.h_a2 = 0;
            _cor.h_b1 = 0;
            _cor.h_b2 = 0;
            _cor.minS = 255;
            _cor.minV = 255;
            _cor.minLuma = 255;
            // cor incorreta
    }

    // Pré-processamento: calcula a mediana dos pixels
    for ( int y = 0; y < cam->altura; y++ ){
        for ( int x = 0; x < cam->largura; x++ ) {
            median( cam->quadro, matrizMediana, y, x, cam->altura, cam->largura );
        }
    }

    // Processa a imagem
    for ( int y = 0; y < cam->altura; y++ ){
        for ( int x = 0; x < cam->largura; x++ ) {

            mR = matrizMediana[y][x][0];
            mG = matrizMediana[y][x][1];
            mB = matrizMediana[y][x][2];

            pR = (mR / POSTERIZE ) * POSTERIZE;
            pG = (mG / POSTERIZE ) * POSTERIZE;
            pB = (mB / POSTERIZE ) * POSTERIZE;

            luma = RGBtoY(&mR, &mG, &mB);

            RGBtoHSV( &mR, &mG, &mB, &mH, &mS, &mV );
            RGBtoHSV( &pR, &pG, &pB, &pH, &pS, &pV );
            mS *= 255;

            if ( ((pH >= _cor.h_a1 && pH <= _cor.h_a2) || (pH >= _cor.h_b1 && pH <= _cor.h_b2)) && mS >= _cor.minS && luma >= _cor.minLuma && pV >= _cor.minV )
                valorPixel = 255;
            else
                valorPixel = 0;

            matrizRGB[y][x][0] = pR;
            matrizRGB[y][x][1] = pG;
            matrizRGB[y][x][2] = pB;

            processaMatriz( valorPixel, y, x );
        }
    }

    //desenhaImagem();

    if ( frame >= 6 )
      frame = 0;

    frame++;
}
//------------------------------------------------------------------------------
void imageProc_atualizaXY( int *x, int *y, FaixaCor_t c ){
    processaImagem( c );

    int ci = 0, cj = 0, cn = 0;

    for(int i = 0; i < cam->altura; i++)
        for(int j = 0; j < cam->largura; j++){

        if( matrizProcessada[i][j][0] > 0 ){
            ci += i;
            cj += j;
            cn++;
        }
    }

/*3 - RESPOSTA*/
    if(cn > 0){
        *y = ci / cn;
        *x = cj / cn;
    }
    else{
        *y = -1;
        *x = -1;
    }
}
//------------------------------------------------------------------------------
void imageProc_finaliza( void ){

    camera_libera_matriz(cam, matrizFrame1);
    camera_libera_matriz(cam, matrizFrame2);
    camera_libera_matriz(cam, matrizFrame3);
    camera_libera_matriz(cam, matrizFrame4);
    camera_libera_matriz(cam, matrizFrame5);
    camera_libera_matriz(cam, matrizFrame6);
    camera_libera_matriz(cam, matriz);
    camera_libera_matriz(cam, matrizRGB);
    camera_libera_matriz(cam, matrizProcessada);
    camera_libera_matriz(cam, matrizMediana);

    camera_finaliza(cam);
}
//------------------------------------------------------------------------------
