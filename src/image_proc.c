#include <allegro5/allegro.h>

#include "camera.h"
#include "estrutura_cam.h"
#include "cor.h"
#include "plantgoshi_arduino.h"

#define POSTERIZE 100
#define BUFFER_FRAMES 3

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
Cor vermelho, azul, verde, amarelo, laranja, ciano, magenta, corAtual;

ArduinoComm arduinoComm;
FaixaCor_t arduinoCorAtual;

int frame;
//------------------------------------------------------------------------------
int analisaFrameExterior( unsigned char ***m, int x1, int x2, int y1, int y2 ){
    int contagem = 0;

    for ( int i = 0; i < cam->altura; i++ ){
        for ( int j = 0; j < cam->largura; j++ ){
            if ( !(i >= y1 && i <= y2 && j >= x1 && j <= x2) ){
                if ( m[i][j][0] > 0 ){
//                    printf("x1: %d   x2: %d  y1: %d  y2: %d\n", x1, x2, y1, y2 );
                    contagem++;
                }
            }
        }
    }

    return contagem;
}
//------------------------------------------------------------------------------
int analisaFrameInterior( unsigned char ***m, int x1, int x2, int y1, int y2 ){
    int contagem = 0;

    for ( int i = 0; i < cam->altura; i++ ){
        for ( int j = 0; j < cam->largura; j++ ){
            if ( i >= y1 && i <= y2 && j >= x1 && j <= x2 ){
                if ( m[i][j][0] > 0 )
                    contagem++;
            }
        }
    }

    return contagem;
}
//------------------------------------------------------------------------------
void getCor( FaixaCor_t faixaCor ){

    switch ( faixaCor ){
        case VERMELHO:
            corAtual.h_a1 = vermelho.h_a1;
            corAtual.h_a2 = vermelho.h_a2;
            corAtual.h_b1 = vermelho.h_b1;
            corAtual.h_b2 = vermelho.h_b2;
            corAtual.minS = vermelho.minS;
            corAtual.minV = vermelho.minV;
            corAtual.minLuma = vermelho.minLuma;
            corAtual.calibrada = vermelho.calibrada;
            corAtual.numTentativas = vermelho.numTentativas;
            corAtual.maxTentativas = vermelho.maxTentativas;
        break;
        case VERDE:
            corAtual.h_a1 = verde.h_a1;
            corAtual.h_a2 = verde.h_a2;
            corAtual.h_b1 = verde.h_b1;
            corAtual.h_b2 = verde.h_b2;
            corAtual.minS = verde.minS;
            corAtual.minV = verde.minV;
            corAtual.minLuma = verde.minLuma;
            corAtual.calibrada = verde.calibrada;
            corAtual.numTentativas = verde.numTentativas;
            corAtual.maxTentativas = verde.maxTentativas;
        break;
        case AZUL:
            corAtual.h_a1 = azul.h_a1;
            corAtual.h_a2 = azul.h_a2;
            corAtual.h_b1 = azul.h_b1;
            corAtual.h_b2 = azul.h_b2;
            corAtual.minS = azul.minS;
            corAtual.minV = azul.minV;
            corAtual.minLuma = azul.minLuma;
            corAtual.calibrada = azul.calibrada;
            corAtual.numTentativas = azul.numTentativas;
            corAtual.maxTentativas = azul.maxTentativas;
        break;
        case AMARELO:
            corAtual.h_a1 = amarelo.h_a1;
            corAtual.h_a2 = amarelo.h_a2;
            corAtual.h_b1 = amarelo.h_b1;
            corAtual.h_b2 = amarelo.h_b2;
            corAtual.minS = amarelo.minS;
            corAtual.minV = amarelo.minV;
            corAtual.minLuma = amarelo.minLuma;
            corAtual.calibrada = amarelo.calibrada;
            corAtual.numTentativas = amarelo.numTentativas;
            corAtual.maxTentativas = amarelo.maxTentativas;
        break;
        default:
            corAtual.h_a1 = 0;
            corAtual.h_a2 = 0;
            corAtual.h_b1 = 0;
            corAtual.h_b2 = 0;
            corAtual.minS = 255;
            corAtual.minV = 255;
            corAtual.minLuma = 255;
            corAtual.calibrada = 0;
            corAtual.numTentativas = 0;
            corAtual.maxTentativas = 10;

            // cor incorreta
    }
}
//------------------------------------------------------------------------------
void setCor( FaixaCor_t faixaCor ){

    switch ( faixaCor ){
        case VERMELHO:
            vermelho.h_a1 = corAtual.h_a1;
            vermelho.h_a2 = corAtual.h_a2;
            vermelho.h_b1 = corAtual.h_b1;
            vermelho.h_b2 = corAtual.h_b2;
            vermelho.minS = corAtual.minS;
            vermelho.minV = corAtual.minV;
            vermelho.minLuma = corAtual.minLuma;
            vermelho.calibrada = corAtual.calibrada;
            vermelho.numTentativas = corAtual.numTentativas;
            vermelho.maxTentativas = corAtual.maxTentativas;
        break;
        case VERDE:
            verde.h_a1 = corAtual.h_a1;
            verde.h_a2 = corAtual.h_a2;
            verde.h_b1 = corAtual.h_b1;
            verde.h_b2 = corAtual.h_b2;
            verde.minS = corAtual.minS;
            verde.minV = corAtual.minV;
            verde.minLuma = corAtual.minLuma;
            verde.calibrada = corAtual.calibrada;
            verde.numTentativas = corAtual.numTentativas;
            verde.maxTentativas = corAtual.maxTentativas;
        break;
        case AZUL:
            azul.h_a1 = corAtual.h_a1;
            azul.h_a2 = corAtual.h_a2;
            azul.h_b1 = corAtual.h_b1;
            azul.h_b2 = corAtual.h_b2;
            azul.minS = corAtual.minS;
            azul.minV = corAtual.minV;
            azul.minLuma = corAtual.minLuma;
            azul.calibrada = corAtual.calibrada;
            azul.numTentativas = corAtual.numTentativas;
            azul.maxTentativas = corAtual.maxTentativas;
        break;
        case AMARELO:
            amarelo.h_a1 = corAtual.h_a1;
            amarelo.h_a2 = corAtual.h_a2;
            amarelo.h_b1 = corAtual.h_b1;
            amarelo.h_b2 = corAtual.h_b2;
            amarelo.minS = corAtual.minS;
            amarelo.minV = corAtual.minV;
            amarelo.minLuma = corAtual.minLuma;
            amarelo.calibrada = corAtual.calibrada;
            amarelo.numTentativas = corAtual.numTentativas;
            amarelo.maxTentativas = corAtual.maxTentativas;
        break;
        default:
            corAtual.h_a1 = 0;
            corAtual.h_a2 = 0;
            corAtual.h_b1 = 0;
            corAtual.h_b2 = 0;
            corAtual.minS = 255;
            corAtual.minV = 255;
            corAtual.minLuma = 255;
            corAtual.calibrada = 0;
            // cor incorreta
    }
}
//------------------------------------------------------------------------------
void imageProc_calibraCor( FaixaCor_t faixaCor, int x1, int x2, int y1, int y2 ){
//    int totalPixelsFrame = cam->largura * cam->altura;
    int numPixelsExterior = analisaFrameExterior( matrizProcessada, x1, x2, y1, y2 );
    int numPixelsInterior = analisaFrameInterior( matrizProcessada, x1, x2, y1, y2 );

    getCor( faixaCor ); // seleciona  corAtual (variavel global)


//    printf("%d\n", corAtual.calibrada);
    if ( numPixelsInterior < 10 && numPixelsExterior < 10 && !corAtual.calibrada && corAtual.numTentativas > 0 ){ // caso tenha passado do angulo de detecção
//        corAtual.calibrada = 1;
        corAtual.h_a1 -= 1;
        corAtual.h_a2 += 1;
        corAtual.h_b1 -= 1;
        corAtual.h_b2 += 1;
        // if ( corAtual.minLuma > 0 && corAtual.minS ){
        //     corAtual.minLuma -= 5;
        //     corAtual.minS -= 5;
        // }
        corAtual.minLuma += 1;
        corAtual.minS += 3;
        setCor( faixaCor ); // salva corAtual (variavel global)
        printf( "---Tentativas: %d\n h_a1: %.3f  h_a2: %.3f\n", corAtual.numTentativas, corAtual.h_a1, corAtual.h_a2 );
        printf("Interior: %d\n", numPixelsInterior);
        printf("Exterior: %d\n", numPixelsExterior);
        printf("Luma: %d\n", corAtual.minLuma);
        printf("Sat: %.3f\n", corAtual.minS);
        printf( "---Cor re-calibrada\n" );
    }

    if ( corAtual.calibrada == 1 || corAtual.numTentativas >= corAtual.maxTentativas )
        return;

    if ( (numPixelsExterior < 50 && numPixelsInterior >= 100 ) ){ // 2%
        corAtual.calibrada = 1;
        printf( "---Cor calibrada\n" );
    }
    else {
        if ( numPixelsInterior >= 100 && numPixelsExterior >= 50 ){
            if ( corAtual.h_a2 - corAtual.h_a1 >= 30 ) {
                corAtual.h_a1 += 1;
                corAtual.h_a2 -= 1;
                corAtual.h_b1 += 1;
                corAtual.h_b2 -= 1;
            }
            else {
                corAtual.minLuma += 5;
                corAtual.minS += 5;
            }

            corAtual.numTentativas += 1;
            printf( "---Tentativas: %d\n h_a1: %.3f  h_a2: %.3f\n", corAtual.numTentativas, corAtual.h_a1, corAtual.h_a2 );
            printf("Interior: %d\n", numPixelsInterior);
            printf("Exterior: %d\n", numPixelsExterior);
            printf("Luma: %d\n", corAtual.minLuma);
            printf("Sat: %.3f\n", corAtual.minS);
        }
    }

    setCor( faixaCor ); // salva corAtual (variavel global)
}
//------------------------------------------------------------------------------
void setupArduino( void ) {

    // Configura a comunicação com o Arduino
    strcpy( arduinoComm.serialPort, "/dev/cu.usbmodem1411" );
    arduinoComm.fd         = -1;
    arduinoComm.baudRate   = 9600;
    arduinoComm.timeOut    = 5000;
    arduinoComm.endOfLine  = '\n';
}
//------------------------------------------------------------------------------
void arduino_setLEDColor( FaixaCor_t c ){

        unsigned char r, g, b;
        arduinoCorAtual = c;

        switch ( c ){
            case VERMELHO:
                r = 255;
                g = 10;
                b = 10;
            break;
            case AZUL:
                r = 10;
                g = 10;
                b = 255;
            break;
            case VERDE:
                r = 10;
                g = 255;
                b = 10;
            break;
            case AMARELO:
                r = 255;
                g = 10;
                b = 255;
            break;
            default:
                r = 100;
                g = 100;
                b = 100;
        }
        arduinoComm.buffer[ 0 ] = b;
        arduinoComm.buffer[ 1 ] = g;
        arduinoComm.buffer[ 2 ] = r;
        arduinoComm.buffer[ 3 ] = '\0';

    //    strcpy( arduinoComm.buffer, "zaz" );

        arduino_LED( &arduinoComm );
}
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
    vermelho.h_a2 = 70;
    vermelho.h_b1 = 330;
    vermelho.h_b2 = 360;
    vermelho.minS = 100;
    vermelho.minV = 0;
    vermelho.minLuma = 60;
    vermelho.calibrada = 0;
    vermelho.nome = VERMELHO;
    vermelho.numTentativas = 0;
    vermelho.maxTentativas = 200;

    azul.h_a1 = 180;
    azul.h_a2 = 270;
    azul.h_b1 = 180;
    azul.h_b2 = 270;
    azul.minS = 0;
    azul.minV = 0;
    azul.minLuma = 0;
    azul.calibrada = 0;
    azul.nome = AZUL;
    azul.numTentativas = 0;
    azul.maxTentativas = 200;

    verde.h_a1 = 60;
    verde.h_a2 = 200;
    verde.h_b1 = 60;
    verde.h_b2 = 200;
    verde.minS = 0;
    verde.minV = 0;
    verde.minLuma = 0;
    verde.calibrada = 0;
    verde.nome = VERDE;
    verde.numTentativas = 0;
    verde.maxTentativas = 200;

    amarelo.h_a1 = 45;
    amarelo.h_a2 = 75;
    amarelo.h_b1 = 45;
    amarelo.h_b2 = 75;
    amarelo.minS = 150;
    amarelo.minV = 0;
    amarelo.minLuma = 50;
    amarelo.calibrada = 0;
    amarelo.nome = AMARELO;
    amarelo.numTentativas = 0;
    amarelo.maxTentativas = 200;

    frame = 1;

    setupArduino();

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
            if ( (i > 0) & (j > 0) & (i < altura) & (j < largura) ){
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
    int divisao = frame % BUFFER_FRAMES;

    switch ( divisao ){
        case 1:
            matrizFrame1[y][x][0] = valorPixel;
            matrizFrame1[y][x][1] = valorPixel;
            matrizFrame1[y][x][2] = valorPixel;
        break;
        case 2:
            matrizFrame2[y][x][0] = valorPixel;
            matrizFrame2[y][x][1] = valorPixel;
            matrizFrame2[y][x][2] = valorPixel;
        break;
        case 3:
            matrizFrame3[y][x][0] = valorPixel;
            matrizFrame3[y][x][1] = valorPixel;
            matrizFrame3[y][x][2] = valorPixel;
        break;
        case 4:
            matrizFrame4[y][x][0] = valorPixel;
            matrizFrame4[y][x][1] = valorPixel;
            matrizFrame4[y][x][2] = valorPixel;
        break;
        case 5:
            matrizFrame5[y][x][0] = valorPixel;
            matrizFrame5[y][x][1] = valorPixel;
            matrizFrame5[y][x][2] = valorPixel;
        break;
        case 0:
            matrizFrame6[y][x][0] = valorPixel;
            matrizFrame6[y][x][1] = valorPixel;
            matrizFrame6[y][x][2] = valorPixel;
        break;
    }
    // buffer de 6 frames
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
void processaImagem( FaixaCor_t faixaCor ){

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

    getCor( faixaCor );

    if ( arduinoCorAtual != faixaCor){
        arduino_setLEDColor( faixaCor );
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

            if ( ((pH >= corAtual.h_a1 & pH <= corAtual.h_a2) || (pH >= corAtual.h_b1 & pH <= corAtual.h_b2)) & (mS >= corAtual.minS) & (luma >= corAtual.minLuma) & (pV >= corAtual.minV) )
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

    if ( frame >= BUFFER_FRAMES )
      frame = 0;

    frame++;
}
//------------------------------------------------------------------------------
void imageProc_atualizaXY( int *x, int *y, FaixaCor_t faixaCor ){
    processaImagem( faixaCor );

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
