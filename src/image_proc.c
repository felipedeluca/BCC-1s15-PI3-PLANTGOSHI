
#include <allegro5/allegro.h>

#include "camera.h"
#include "estrutura_cam.h"
#include "cor.h"
#include "plantgoshi_arduino.h"

#define POSTERIZE 1
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

//int posterizeRatio;
int frame, calibrarCor;

void setupArduino( void );
//------------------------------------------------------------------------------
void imageProc_init( int calibrar ){


    cam = camera_inicializa(0);

    if(!cam){
        fprintf(stderr, "%s\n", "nao foi possivel inicializar camera");
        exit(EXIT_FAILURE);
    }

    calibrarCor = calibrar;
    corAtual.posterizeRatio = 1;

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
    vermelho.h_b1 = 320;
    vermelho.h_b2 = 360;
    vermelho.minS = -0.01;
    vermelho.minV = -0.01;
    vermelho.maxS = 1;
    vermelho.maxV = 0;
    vermelho.maxLuma = 255;
    vermelho.minLuma = -1;
    vermelho.calibrada = 0;
    vermelho.tipo = VERMELHO;
    vermelho.numTentativas = 0;
    vermelho.maxTentativas = 100;
    vermelho.posterizeRatio = 100;

    azul.h_a1 = 200;
    azul.h_a2 = 270;
    azul.h_b1 = 200;
    azul.h_b2 = 270;
    azul.minS = -0.01;
    azul.minV = -0.01;
    azul.maxS = 1;
    azul.maxV = 0;
    azul.maxLuma = 255;
    azul.minLuma = -1;
    azul.calibrada = 0;
    azul.tipo = AZUL;
    azul.numTentativas = 0;
    azul.maxTentativas = 100;
    azul.posterizeRatio = 1;

    verde.h_a1 = 60;
    verde.h_a2 = 200;
    verde.h_b1 = 60;
    verde.h_b2 = 200;
    verde.minS = -0.01;
    verde.minV = -0.01;
    verde.maxS = 1;
    verde.maxV = 0;
    verde.maxLuma = 255;
    verde.minLuma = -1;
    verde.calibrada = 0;
    verde.tipo = VERDE;
    verde.numTentativas = 0;
    verde.maxTentativas = 100;
    verde.posterizeRatio = 1;

    amarelo.h_a1 = 35;
    amarelo.h_a2 = 75;
    amarelo.h_b1 = 35;
    amarelo.h_b2 = 75;
    amarelo.minS = -0.01;
    amarelo.minV = -0.01;
    amarelo.maxS = 1;
    amarelo.maxV = 0;
    amarelo.maxLuma = 255;
    amarelo.minLuma = -1;
    amarelo.calibrada = 0;
    amarelo.tipo = AMARELO;
    amarelo.numTentativas = 0;
    amarelo.maxTentativas = 100;
    amarelo.posterizeRatio = 1;

    ciano.h_a1 = 160;
    ciano.h_a2 = 200;
    ciano.h_b1 = 160;
    ciano.h_b2 = 200;
    ciano.minS = -0.01;
    ciano.minV = -0.01;
    ciano.maxS = 1;
    ciano.maxV = 0;
    ciano.maxLuma = 255;
    ciano.minLuma = -1;
    ciano.calibrada = 0;
    ciano.tipo = CIANO;
    ciano.numTentativas = 0;
    ciano.maxTentativas = 100;
    ciano.posterizeRatio = 1;

    magenta.h_a1 = 290;
    magenta.h_a2 = 340;
    magenta.h_b1 = 290;
    magenta.h_b2 = 340;
    magenta.minS = -0.01;
    magenta.minV = -0.01;
    magenta.maxS = 1;
    magenta.maxV = 0;
    magenta.maxLuma = 255;
    magenta.minLuma = -1;
    magenta.calibrada = 0;
    magenta.tipo = MAGENTA;
    magenta.numTentativas = 0;
    magenta.maxTentativas = 100;
    magenta.posterizeRatio = 1;

    frame = 1;

    setupArduino();

}
//------------------------------------------------------------------------------
void escreverArquivo(FaixaCor_t cor){

   FILE *pf;

//   char a = 'a';
   switch (cor){
      case AMARELO:
         pf = fopen("AMARELO.txt", "wb");
         if (pf == NULL) {
            printf( "\nNão encontrei arquivo AMARELO.txt\n");
            exit( EXIT_FAILURE);
         }
         break;
      case VERMELHO:
         pf = fopen("VERMELHO.txt", "wb");
         if (pf == NULL) {
            printf( "\nNão encontrei arquivo VERMELHO.txt\n");
            exit( EXIT_FAILURE);
         }
         break;
      case AZUL:
         pf = fopen("AZUL.txt", "wb");
         if (pf == NULL) {
            printf( "\nNão encontrei arquivo AZUL.txt\n");
            exit( EXIT_FAILURE);
         }
         break;
      case VERDE:
         pf = fopen("VERDE.txt", "wb");
         if (pf == NULL) {
            printf( "\nNão encontrei arquivo VERDE.txt\n");
            exit( EXIT_FAILURE);
         }
         break;
       case CIANO:
            pf = fopen("CIANO.txt", "wb");
            if (pf == NULL) {
                printf( "\nNão encontrei arquivo CIANO.txt\n");
                exit( EXIT_FAILURE);
            }
       break;
       case MAGENTA:
            pf = fopen("MAGENTA.txt", "wb");
            if (pf == NULL) {
                printf( "\nNão encontrei arquivo MAGENTA.txt\n");
                exit( EXIT_FAILURE);
             }
       break;
      default:
        return;
   }
   fprintf(pf, "%f ", corAtual.h_a1);
   fprintf(pf, "%f ", corAtual.h_a2);
   fprintf(pf, "%f ", corAtual.h_b1);
   fprintf(pf, "%f ", corAtual.h_b2);
   fprintf(pf, "%f ", corAtual.minS);
   fprintf(pf, "%f ", corAtual.maxS);
   fprintf(pf, "%f ", corAtual.minV);
   fprintf(pf, "%f ", corAtual.maxV);
   fprintf(pf, "%f ", corAtual.minLuma);
   fprintf(pf, "%f ", corAtual.maxLuma);
   fprintf(pf, "%d ", corAtual.posterizeRatio);

   fclose(pf); /* Fecha o arquivo */
}
//------------------------------------------------------------------------------
void lerArquivo(FaixaCor_t cor){

 //  int k= 1, a, b, c, d, e;
   FILE *pf;

   switch (cor){
      case AMARELO:
         pf = fopen("config/AMARELO.txt", "r");
         break;
      case VERMELHO:
         pf = fopen("config/VERMELHO.txt", "r");
         break;
      case AZUL:
         pf = fopen("config/AZUL.txt", "r");
         break;
      case VERDE:
         pf = fopen("config/VERDE.txt", "r");
         break;
      case MAGENTA:
         pf = fopen("config/MAGENTA.txt", "r");
         break;
      case CIANO:
         pf = fopen("config/CIANO.txt", "r");
         break;
      default:
        return;
   }

   if ( pf == NULL){
       printf("Não foi possível carregar a configuração da cor.\n");
       return;
       fclose(pf);
   }

   fscanf(pf, "%f", &corAtual.h_a1);
   fscanf(pf, "%f", &corAtual.h_a2);
   fscanf(pf, "%f", &corAtual.h_b1);
   fscanf(pf, "%f", &corAtual.h_b2);
   fscanf(pf, "%f", &corAtual.minS);
   fscanf(pf, "%f", &corAtual.maxS);
   fscanf(pf, "%f", &corAtual.minV);
   fscanf(pf, "%f", &corAtual.maxV);
   fscanf(pf, "%f", &corAtual.minLuma);
   fscanf(pf, "%f", &corAtual.maxLuma);
   fscanf(pf, "%d", &corAtual.posterizeRatio);

   // fscanf(pf, "%f", &a);
   // fscanf(pf, "%f", &b);
   // fscanf(pf, "%f", &c);
   // fscanf(pf, "%f", &d);
   // fscanf(pf, "%d", &e);
   // fscanf(pf, "%d", &e);

   printf("ha1: %.2f, ha2: %.2f, hb1: %.2f, hb2: %.2f, minL: %.3f, maxL: %.3f, minS: %.3f, maxS: %.3f\n", corAtual.h_a1,
            corAtual.h_a2, corAtual.h_b1, corAtual.h_b2, corAtual.minLuma, corAtual.maxLuma, corAtual.minS, corAtual.maxS );

   fclose(pf);
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
            corAtual.maxS = vermelho.maxS;
            corAtual.maxV = vermelho.maxV;
            corAtual.minLuma = vermelho.minLuma;
            corAtual.maxLuma = vermelho.maxLuma;
            corAtual.calibrada = vermelho.calibrada;
            corAtual.numTentativas = vermelho.numTentativas;
            corAtual.maxTentativas = vermelho.maxTentativas;
            corAtual.tipo = faixaCor;
            corAtual.posterizeRatio = vermelho.posterizeRatio;
        //    posterizeRatio = 100;
        break;
        case VERDE:
            corAtual.h_a1 = verde.h_a1;
            corAtual.h_a2 = verde.h_a2;
            corAtual.h_b1 = verde.h_b1;
            corAtual.h_b2 = verde.h_b2;
            corAtual.minS = verde.minS;
            corAtual.minV = verde.minV;
            corAtual.maxS = verde.maxS;
            corAtual.maxV = verde.maxV;
            corAtual.minLuma = verde.minLuma;
            corAtual.maxLuma = verde.maxLuma;
            corAtual.calibrada = verde.calibrada;
            corAtual.numTentativas = verde.numTentativas;
            corAtual.maxTentativas = verde.maxTentativas;
            corAtual.tipo = faixaCor;
            corAtual.posterizeRatio = verde.posterizeRatio;
        //    posterizeRatio = 1;
        break;
        case AZUL:
            corAtual.h_a1 = azul.h_a1;
            corAtual.h_a2 = azul.h_a2;
            corAtual.h_b1 = azul.h_b1;
            corAtual.h_b2 = azul.h_b2;
            corAtual.minS = azul.minS;
            corAtual.minV = azul.minV;
            corAtual.maxS = azul.maxS;
            corAtual.maxV = azul.maxV;
            corAtual.minLuma = azul.minLuma;
            corAtual.maxLuma = azul.maxLuma;
            corAtual.calibrada = azul.calibrada;
            corAtual.numTentativas = azul.numTentativas;
            corAtual.maxTentativas = azul.maxTentativas;
            corAtual.tipo = faixaCor;
            corAtual.posterizeRatio = azul.posterizeRatio;
        //    posterizeRatio = 1;
        break;
        case AMARELO:
            corAtual.h_a1 = amarelo.h_a1;
            corAtual.h_a2 = amarelo.h_a2;
            corAtual.h_b1 = amarelo.h_b1;
            corAtual.h_b2 = amarelo.h_b2;
            corAtual.minS = amarelo.minS;
            corAtual.minV = amarelo.minV;
            corAtual.maxS = amarelo.maxS;
            corAtual.maxV = amarelo.maxV;
            corAtual.minLuma = amarelo.minLuma;
            corAtual.maxLuma = amarelo.maxLuma;
            corAtual.calibrada = amarelo.calibrada;
            corAtual.numTentativas = amarelo.numTentativas;
            corAtual.maxTentativas = amarelo.maxTentativas;
            corAtual.tipo = faixaCor;
            corAtual.posterizeRatio = amarelo.posterizeRatio;
        //    posterizeRatio = 1;
        break;
        case CIANO:
            corAtual.h_a1 = ciano.h_a1;
            corAtual.h_a2 = ciano.h_a2;
            corAtual.h_b1 = ciano.h_b1;
            corAtual.h_b2 = ciano.h_b2;
            corAtual.minS = ciano.minS;
            corAtual.minV = ciano.minV;
            corAtual.maxS = ciano.maxS;
            corAtual.maxV = ciano.maxV;
            corAtual.minLuma = ciano.minLuma;
            corAtual.maxLuma = ciano.maxLuma;
            corAtual.calibrada = ciano.calibrada;
            corAtual.numTentativas = ciano.numTentativas;
            corAtual.maxTentativas = ciano.maxTentativas;
            corAtual.tipo = faixaCor;
            corAtual.posterizeRatio = ciano.posterizeRatio;
        //    posterizeRatio = 1;
        break;
        case MAGENTA:
            corAtual.h_a1 = magenta.h_a1;
            corAtual.h_a2 = magenta.h_a2;
            corAtual.h_b1 = magenta.h_b1;
            corAtual.h_b2 = magenta.h_b2;
            corAtual.minS = magenta.minS;
            corAtual.minV = magenta.minV;
            corAtual.maxS = magenta.maxS;
            corAtual.maxV = magenta.maxV;
            corAtual.minLuma = magenta.minLuma;
            corAtual.maxLuma = magenta.maxLuma;
            corAtual.calibrada = magenta.calibrada;
            corAtual.numTentativas = magenta.numTentativas;
            corAtual.maxTentativas = magenta.maxTentativas;
            corAtual.tipo = faixaCor;
            corAtual.posterizeRatio = magenta.posterizeRatio;
        //    posterizeRatio = 1;
        break;
        default:
            corAtual.h_a1 = 0;
            corAtual.h_a2 = 0;
            corAtual.h_b1 = 0;
            corAtual.h_b2 = 0;
            corAtual.minS = 255;
            corAtual.minV = 255;
            corAtual.maxS = 255;
            corAtual.maxV = 255;
            corAtual.minLuma = 255;
            corAtual.maxLuma = 255;
            corAtual.calibrada = 0;
            corAtual.numTentativas = 0;
            corAtual.maxTentativas = 10;
            corAtual.tipo = faixaCor;
            corAtual.posterizeRatio = 1;
        //    posterizeRatio = 100;
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
            vermelho.maxS = corAtual.maxS;
            vermelho.maxV = corAtual.maxV;
            vermelho.minLuma = corAtual.minLuma;
            vermelho.maxLuma = corAtual.maxLuma;
            vermelho.calibrada = corAtual.calibrada;
            vermelho.numTentativas = corAtual.numTentativas;
            vermelho.maxTentativas = corAtual.maxTentativas;
            vermelho.posterizeRatio = corAtual.posterizeRatio;
        break;
        case VERDE:
            verde.h_a1 = corAtual.h_a1;
            verde.h_a2 = corAtual.h_a2;
            verde.h_b1 = corAtual.h_b1;
            verde.h_b2 = corAtual.h_b2;
            verde.minS = corAtual.minS;
            verde.minV = corAtual.minV;
            verde.maxS = corAtual.maxS;
            verde.maxV = corAtual.maxV;
            verde.minLuma = corAtual.minLuma;
            verde.maxLuma = corAtual.maxLuma;
            verde.calibrada = corAtual.calibrada;
            verde.numTentativas = corAtual.numTentativas;
            verde.maxTentativas = corAtual.maxTentativas;
            verde.posterizeRatio = corAtual.posterizeRatio;
        break;
        case AZUL:
            azul.h_a1 = corAtual.h_a1;
            azul.h_a2 = corAtual.h_a2;
            azul.h_b1 = corAtual.h_b1;
            azul.h_b2 = corAtual.h_b2;
            azul.minS = corAtual.minS;
            azul.minV = corAtual.minV;
            azul.maxS = corAtual.maxS;
            azul.maxV = corAtual.maxV;
            azul.minLuma = corAtual.minLuma;
            azul.maxLuma = corAtual.maxLuma;
            azul.calibrada = corAtual.calibrada;
            azul.numTentativas = corAtual.numTentativas;
            azul.maxTentativas = corAtual.maxTentativas;
            azul.posterizeRatio = corAtual.posterizeRatio;
        break;
        case AMARELO:
            amarelo.h_a1 = corAtual.h_a1;
            amarelo.h_a2 = corAtual.h_a2;
            amarelo.h_b1 = corAtual.h_b1;
            amarelo.h_b2 = corAtual.h_b2;
            amarelo.minS = corAtual.minS;
            amarelo.minV = corAtual.minV;
            amarelo.maxS = corAtual.maxS;
            amarelo.maxV = corAtual.maxV;
            amarelo.minLuma = corAtual.minLuma;
            amarelo.maxLuma = corAtual.maxLuma;
            amarelo.calibrada = corAtual.calibrada;
            amarelo.numTentativas = corAtual.numTentativas;
            amarelo.maxTentativas = corAtual.maxTentativas;
            amarelo.posterizeRatio = corAtual.posterizeRatio;
        break;
        case CIANO:
            ciano.h_a1 = corAtual.h_a1;
            ciano.h_a2 = corAtual.h_a2;
            ciano.h_b1 = corAtual.h_b1;
            ciano.h_b2 = corAtual.h_b2;
            ciano.minS = corAtual.minS;
            ciano.minV = corAtual.minV;
            ciano.maxS = corAtual.maxS;
            ciano.maxV = corAtual.maxV;
            ciano.minLuma = corAtual.minLuma;
            ciano.maxLuma = corAtual.maxLuma;
            ciano.calibrada = corAtual.calibrada;
            ciano.numTentativas = corAtual.numTentativas;
            ciano.maxTentativas = corAtual.maxTentativas;
            ciano.posterizeRatio = corAtual.posterizeRatio;
        break;
        case MAGENTA:
            magenta.h_a1 = corAtual.h_a1;
            magenta.h_a2 = corAtual.h_a2;
            magenta.h_b1 = corAtual.h_b1;
            magenta.h_b2 = corAtual.h_b2;
            magenta.minS = corAtual.minS;
            magenta.minV = corAtual.minV;
            magenta.maxS = corAtual.maxS;
            magenta.maxV = corAtual.maxV;
            magenta.minLuma = corAtual.minLuma;
            magenta.maxLuma = corAtual.maxLuma;
            magenta.calibrada = corAtual.calibrada;
            magenta.numTentativas  = corAtual.numTentativas;
            magenta.maxTentativas  = corAtual.maxTentativas;
            magenta.posterizeRatio = corAtual.posterizeRatio;
        default:
            corAtual.h_a1 = 0;
            corAtual.h_a2 = 0;
            corAtual.h_b1 = 0;
            corAtual.h_b2 = 0;
            corAtual.minS = 255;
            corAtual.minV = 255;
            corAtual.maxS = 255;
            corAtual.maxV = 255;
            corAtual.minLuma = 255;
            corAtual.maxLuma = 255;
            corAtual.calibrada = 0;
            corAtual.posterizeRatio = 1;
            // cor incorreta
    }
}
//------------------------------------------------------------------------------
void imageProc_carregaCores( void ){

    lerArquivo( VERDE );
    setCor( VERDE );

    lerArquivo( VERMELHO );
    setCor( VERMELHO );

    lerArquivo( AZUL );
    setCor( AZUL );

    lerArquivo( MAGENTA );
    setCor( MAGENTA );

    lerArquivo( AMARELO );
    setCor( AMARELO );

    lerArquivo( CIANO );
    setCor( CIANO );
}
//------------------------------------------------------------------------------
void setupArduino( void ) {

    // Configura a comunicação com o Arduino
    strcpy( arduinoComm.serialPort, "/dev/cu.usbmodem1411" );
    arduinoComm.fd         = -1;
    arduinoComm.baudRate   = 115200;
    arduinoComm.timeOut    = 5000;
    arduinoComm.endOfLine  = '\n';
}
//------------------------------------------------------------------------------
void arduino_setLEDColor( FaixaCor_t cor ){

//    printf("%d\n", cor);

        char setColor = '6';
        switch ( cor ){
            case AMARELO:
                setColor = '1';
            break;
            case VERDE:
                setColor = '2';
            break;
            case CIANO:
                setColor = '3';
            break;
            case AZUL:
                setColor = '4';
            break;
            case MAGENTA:
                setColor = '5';
            break;
            case VERMELHO:
                setColor = '6';
            break;
            default:
                setColor = '7';
        }

         arduinoComm.buffer[ 0 ] = setColor;
         arduinoComm.buffer[ 1 ] = setColor;
         arduinoComm.buffer[ 2 ] = setColor;
         arduinoComm.buffer[ 3 ] = '\0';

        //strcpy( arduinoComm.buffer, "zzz" );

       arduino_LED( &arduinoComm );
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
    output[y][x][0] = (sumR / 49);
    output[y][x][1] = (sumG / 49);
    output[y][x][2] = (sumB / 49);
}
//------------------------------------------------------------------------------
unsigned char erode( unsigned char ***m, int y, int x, int altura, int largura ){
	int left = 0, right = 0, top = 0, bottom = 0, topLeft = 0, bottomLeft = 0, topRight = 0, bottomRight = 0;

    int x1 = x - 1;
    int x2 = x + 1;
    int y1 = y - 1;
    int y2 = y + 1;

	if ( (y1 < 0) | (m[y1][x][0] > 0) ){
        top = 1;
    }
    printf("Entrei\n");
	if ( (y2 >= altura) | (m[y2][x][0] > 0) ){
        bottom = 1;
    }
	if ( (x1 < 0) | (m[y][x1][0] > 0) ){
        left = 1;
    }
	if ( (x2 >= largura) | (m[y][x2][0] > 0) ){
        right = 1;
    }
    if ( ((y1 < 0) | (m[y1][x][0] > 0)) & ((x1 < 0) | (m[y][x1][0] > 0)) ) {
        topLeft = 1;
    }
    if ( ((y1 < 0) | (m[y1][x][0] > 0)) & ((x2 >= largura) | (m[y][x2][0] > 0)) ) {
        topRight = 1;
    }
    if ( ((y2 >= altura) | (m[y2][x][0] > 0)) & ((x1 < 0) | (m[y][x1][0] > 0)) ) {
        bottomLeft = 1;
    }
    if ( ((y2 >= altura) | (m[y2][x][0] > 0)) & ((x2 >= largura) | (m[y][x2][0] > 0)) ) {
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
float RGBtoY( unsigned char *r, unsigned char *g, unsigned char *b ) {
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
    // matriz[y][x][0] = ( matrizFrame1[y][x][0] | matrizFrame2[y][x][0] | matrizFrame3[y][x][0] | matrizFrame4[y][x][0] | matrizFrame5[y][x][0] | matrizFrame6[y][x][0] );
    // matriz[y][x][1] = ( matrizFrame1[y][x][1] | matrizFrame2[y][x][1] | matrizFrame3[y][x][1] | matrizFrame4[y][x][1] | matrizFrame5[y][x][1] | matrizFrame6[y][x][1] );
    // matriz[y][x][2] = ( matrizFrame1[y][x][2] | matrizFrame2[y][x][2] | matrizFrame3[y][x][2] | matrizFrame4[y][x][2] | matrizFrame5[y][x][2] | matrizFrame6[y][x][2] );

    matrizProcessada[y][x][0] = ( matrizFrame1[y][x][0] | matrizFrame2[y][x][0] | matrizFrame3[y][x][0] );// | matrizFrame4[y][x][0] | matrizFrame5[y][x][0] | matrizFrame6[y][x][0] );
    matrizProcessada[y][x][1] = ( matrizFrame1[y][x][1] | matrizFrame2[y][x][1] | matrizFrame3[y][x][1] );// | matrizFrame4[y][x][1] | matrizFrame5[y][x][1] | matrizFrame6[y][x][1] );
    matrizProcessada[y][x][2] = ( matrizFrame1[y][x][2] | matrizFrame2[y][x][2] | matrizFrame3[y][x][2] );// | matrizFrame4[y][x][2] | matrizFrame5[y][x][2] | matrizFrame6[y][x][2] );
    // median( matriz, matrizPreProcessada, y, x, cam->altura, cam->largura );

    //unsigned char e = erode( matriz, y, x, cam->altura, cam->largura );
    //unsigned char e = matriz[y][x][0];
    // matrizProcessada[y][x][0] = e;
    // matrizProcessada[y][x][1] = e;
    // matrizProcessada[y][x][2] = e;
}
//------------------------------------------------------------------------------
void imageProc_desenhaImagem( ALLEGRO_BITMAP *telaEsquerda, ALLEGRO_BITMAP *telaDireita ){

    camera_copia(cam, matrizProcessada, telaEsquerda);
    camera_copia(cam, matrizRGB, telaDireita);

}
//------------------------------------------------------------------------------
int analisaFrameExterior( unsigned char ***m, int x1, int x2, int y1, int y2 ){
    int contagem = 0;

    for ( int i = 0; i < cam->altura; i++ ){
        for ( int j = 0; j < cam->largura; j++ ){
            if ( !((i >= y1) & (i <= y2) & (j >= x1) & (j <= x2)) ){
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
            if ( (i >= y1) & (i <= y2) & (j >= x1) & (j <= x2) ){
                if ( m[i][j][0] > 0 )
                    contagem++;
            }
        }
    }

    return contagem;
}
//------------------------------------------------------------------------------
void analisaMinMaxFrameInterior( int x1, int x2, int y1, int y2, int i, int j, float mS, float luma ){

    if ( luma <= 0 )
        return;

    if ( i >= y1 && i <= y2 && j >= x1 && j <= x2 ){
        if ( (corAtual.minS <= 0) | (( mS / 2 ) > corAtual.minS) )
            corAtual.minS += 0.001;
        else if ( (corAtual.maxS == 1) | (( mS / 2 ) + corAtual.minS <= corAtual.maxS) )
            corAtual.maxS -= 0.00001;

        if ( (corAtual.minLuma <= 0) | (( luma / 2 ) > corAtual.minLuma) )
            corAtual.minLuma += 0.1;
        else if ( (corAtual.maxLuma == 255) | (( luma / 2 ) + corAtual.minLuma <= corAtual.maxLuma) )
            corAtual.maxLuma -= 0.5;

            printf("luma: %.3f  minLuma: %.3f   maxLuma: %.3f   sat: %.3f  minS: %.3f  maxS: %.3f\n", luma, corAtual.minLuma, corAtual.maxLuma, mS, corAtual.minS, corAtual.maxS );
    }
}
//------------------------------------------------------------------------------
int imageProc_calibraCor( FaixaCor_t faixaCor, int x1, int x2, int y1, int y2 ){
//    int totalPixelsFrame = cam->largura * cam->altura;

    if ( corAtual.calibrada )
            return 0;

    int numPixelsExterior = analisaFrameExterior( matrizProcessada, x1, x2, y1, y2 );
    int numPixelsInterior = analisaFrameInterior( matrizProcessada, x1, x2, y1, y2 );

    getCor( faixaCor ); // seleciona  corAtual (variavel global)

//    printf("%d\n", corAtual.calibrada);
    if ( (numPixelsInterior > 10) & (numPixelsInterior < 250)  & (numPixelsExterior <= 10) & (!corAtual.calibrada) & (corAtual.numTentativas > 0) ){ // caso tenha passado do angulo de detecção
        corAtual.h_a1 -= 0.2;
        corAtual.h_a2 += 0.2;
        corAtual.h_b1 -= 0.2;
        corAtual.h_b2 += 0.2;
        setCor( faixaCor ); // salva corAtual (variavel global)
        printf( "+Ajustando cor\n" );
    }
    else if ( (numPixelsInterior > 10) & (numPixelsInterior < 300)  & (numPixelsExterior > 150) & (!corAtual.calibrada) & (corAtual.numTentativas > 0) ){ // caso tenha passado do angulo de detecção
        corAtual.minLuma += 0.08;
        corAtual.maxLuma -= 0.1;
        corAtual.minS += 0.005;
        corAtual.maxS -= 0.0001;

        if ( corAtual.tipo != VERMELHO )
            corAtual.posterizeRatio -= 1;

        setCor( faixaCor ); // salva corAtual (variavel global)
        printf( "++Ajustando cor\n" );
    }
    else if ( (numPixelsInterior <= 10) & (numPixelsExterior <= 10) & (!corAtual.calibrada) & (corAtual.numTentativas > 0) ){
        corAtual.h_a1 -= 0.2;
        corAtual.h_a2 += 0.2;
        corAtual.h_b1 -= 0.2;
        corAtual.h_b2 += 0.2;

        if ( corAtual.tipo != VERMELHO )
            corAtual.posterizeRatio -= 1;

        if ( corAtual.posterizeRatio < 1 )
            corAtual.posterizeRatio = 1;
        // corAtual.minLuma -= 0.001;
        // corAtual.maxLuma += 0.001;
        // corAtual.minS -= 0.0000001;
        // corAtual.maxS += 0.0000001;
   }

    if (  (corAtual.numTentativas >= corAtual.maxTentativas) & (!corAtual.calibrada) ){
        if ( corAtual.numTentativas == corAtual.maxTentativas){
            printf( "Número esgotado de tentativas de calibragem!\n" );
            printf( "-- Cor calibrada com imprecisão.!\n" );
            escreverArquivo( faixaCor );
            exit(EXIT_FAILURE);
        }
    }

    if ( (numPixelsExterior < 150) & (numPixelsInterior >= 300) ){ // 2%
        corAtual.calibrada = 1;
        printf( "** Cor calibrada!\n" );
        setCor( faixaCor );
        escreverArquivo( faixaCor );
        return 0;
    }
    else {
        if ( (numPixelsInterior >= 400) & (numPixelsExterior >= 150) ){
            if ( corAtual.h_a2 - corAtual.h_a1 >= 5 ) {
            //    printf( "fechando angulo!\n" );
                corAtual.h_a1 += 0.5;
                corAtual.h_a2 -= 0.5;
                corAtual.h_b1 += 0.5;
                corAtual.h_b2 -= 0.5;

                if ( corAtual.tipo != VERMELHO )
                    corAtual.posterizeRatio += 1;
                else
                    corAtual.posterizeRatio = 100;

                if ( corAtual.posterizeRatio > 100 )
                    corAtual.posterizeRatio = 100;
            }

            corAtual.numTentativas += 1;
        }
    }

    setCor( faixaCor ); // salva corAtual (variavel global)
    return 0;
}
//------------------------------------------------------------------------------
void calculaBrilho( FaixaCor_t cor, float *r, float *g, float *b ){
    switch (cor){
       case AMARELO:
            *r = 0.7;
            *g = 0.7;
            *b = 1.0;
       break;
       case VERMELHO:
            *r = 0.7;
            *g = 1.0;
            *b = 1.0;
       break;
       case AZUL:
           *r = 1.0;
           *g = 1.0;
           *b = 0.7;
       break;
       case VERDE:
           *r = 1.0;
           *g = 0.7;
           *b = 1.0;
       break;
       case MAGENTA:
           *r = 0.7;
           *g = 1.0;
           *b = 0.7;
       break;
       case CIANO:
           *r = 1.0;
           *g = 0.7;
           *b = 0.7;
       break;
       default:
           *r = 0.9;
           *g = 0.9;
           *b = 0.9;
    }
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

    float luma;//

    float mH = 0.0; // mediana matriz
    float mS = 0.0; // mediana saturação
    float mV = 0.0; // mediana valor
    float pH = 0.0; // posterização matiz
    float pS = 0.0; // posterização saturação
    float pV = 0.0; // posterização valor

    unsigned char valorPixel = 0;

    getCor( faixaCor );

    if ( arduinoCorAtual != faixaCor){
        arduino_setLEDColor( faixaCor );
        arduinoCorAtual = faixaCor;
    }

    // Pré-processamento: calcula a mediana dos pixels
    for ( int y = 0; y < cam->altura; y++ ){
        for ( int x = 0; x < cam->largura; x++ ) {
            median( cam->quadro, matrizMediana, y, x, cam->altura, cam->largura );
        }
    }

    float brilhoR, brilhoG, brilhoB; // Valores para manipulação individual dos canais de cores
    calculaBrilho( faixaCor, &brilhoR, &brilhoG, &brilhoB );

    // Processa a imagem
    for ( int y = 0; y < cam->altura; y++ ){
        for ( int x = 0; x < cam->largura; x++ ) {

            mR = matrizMediana[y][x][0] * brilhoR;
            mG = matrizMediana[y][x][1] * brilhoG;
            mB = matrizMediana[y][x][2] * brilhoB;

            pR = (mR / corAtual.posterizeRatio ) * corAtual.posterizeRatio;
            pG = (mG / corAtual.posterizeRatio ) * corAtual.posterizeRatio;
            pB = (mB / corAtual.posterizeRatio ) * corAtual.posterizeRatio;

            luma = RGBtoY(&mR, &mG, &mB);

            RGBtoHSV( &mR, &mG, &mB, &mH, &mS, &mV );
            RGBtoHSV( &pR, &pG, &pB, &pH, &pS, &pV );
            //mS *= 255;
//printf("mS: %f\n", luma);
            if ( (((pH >= corAtual.h_a1) & (pH <= corAtual.h_a2)) | ((pH >= corAtual.h_b1) & (pH <= corAtual.h_b2)))
                   & ((mS >= corAtual.minS) & (mS <= corAtual.maxS) ) & ((luma >= corAtual.minLuma) & (luma <= corAtual.maxLuma)) ){

                if ( (!corAtual.calibrada) & (calibrarCor) )
                        analisaMinMaxFrameInterior( 80, 200, 80, 180, y, x, mS,  luma );

                valorPixel = 255;
            }
            else
                valorPixel = 0;

            matrizRGB[y][x][0] = pR;
            matrizRGB[y][x][1] = pG;
            matrizRGB[y][x][2] = pB;

            processaMatriz( valorPixel, y, x );

        }
    }

    if ( calibrarCor ){
          imageProc_calibraCor( faixaCor, 80, 200, 80, 180 );
    }
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
