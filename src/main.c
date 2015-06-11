/*
================================================================================
    PlantGoshi v1.0

    Centro Universitário Senac

    Bacharelado em Ciência da Computação
    Professor orientador Marcelo Hashimoto
    Projeto Integrador III

    Integrantes do grupo:

        Anderson J. da Silva
        Felipe R. de Luca
        Nelson J. Dressler

    09/06/2015

================================================================================
*/
// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#define MAX 50

#include "image_proc.h"
#include "math.h"
#include "cor.h"
#include "variaveis_fruto.h"
#include "arvore.h"

//#include "plantgoshi_arduino.h"

FaixaCor_t FaixaCor;
const int  LARGURA_TELA  = 1280;
const int  ALTURA_TELA   = 720;
//FPS=frames por segundo - controle de animacao -
const float FPS          = 60.0;
//controle de tela
const int quantidadeFruto = 10;
FaixaCor_t corAtualVarinha;
/*
ArduinoComm arduinoComm;
//------------------------------------------------------------------------------
void setupArduino( void ) {

    // Configura a comunicação com o Arduino
    strcpy( arduinoComm.serialPort, "/dev/cu.usbmodem1421" );
    arduinoComm.fd         = -1;
    arduinoComm.baudRate   = 9600;
    arduinoComm.timeOut    = 5000;
    arduinoComm.endOfLine  = '\n';
}
//------------------------------------------------------------------------------
void erro(char *mensagem) {
  fprintf(stderr, "%s\n", mensagem);

  exit(EXIT_FAILURE);
}
//------------------------------------------------------------------------------
*/
ALLEGRO_EVENT_QUEUE     *fila_eventos   = NULL;
ALLEGRO_TIMER           *timer          = NULL, *contador = NULL;
ALLEGRO_DISPLAY         *janela         = NULL;
ALLEGRO_FONT            *fonte_80       = NULL;

ALLEGRO_BITMAP          *bitmap         = NULL;

ALLEGRO_BITMAP          *nota               = NULL;
ALLEGRO_BITMAP          *notaCursor         = NULL;

ALLEGRO_BITMAP          *cesto              = NULL;
ALLEGRO_BITMAP          *cestoCursor        = NULL;

ALLEGRO_BITMAP          *fogo               = NULL;
ALLEGRO_BITMAP          *fogoCursor         = NULL;

ALLEGRO_BITMAP          *gota               = NULL;
ALLEGRO_BITMAP          *gotaCursor         = NULL;

int  inicializadores ();
void destruir ();
void AtribuindoPosicaoFrutoNovo (Fruta[], int, PosicaoFruto *);
void tempPosicaoNovoNo (Fruta[], int , PosicaoFruto *);
void desenhandoNovoFruto (Fruta[], AllVariaveisJogo *, PosicaoFruto *);
void reiniciarParaNovoJogo(Fruta[], PosicaoFruto *, AllVariaveisJogo *);
void atualizandoPoderEstadoFruto (Fruta[], AllVariaveisJogo *, TempoFruto *, PosicaoFruto *);
void atualizandoFuncaoVarinha (AllVariaveisJogo *);
void reiniciaTempoParaFrutoMudar(TempoFruto *);

//----------------------------------------------------------------------------------------------
void opcoesInicializacao( void ){

    printf("PlantGoshi v. 1.0\n");
    printf("=========================================\n");
    printf("Digite o número da cor a ser utilizada no rastreamento:\n\n");
    printf("1 - amarelo\n");
    printf("2 - verde\n");
    printf("3 - ciano\n");
    printf("4 - azul\n");
    printf("5 - magenta\n");
    printf("6 - vermelho\n\n");

    exit(EXIT_FAILURE);
}
//----------------------------------------------------------------------------------------------
int main ( int argc, char **argv ) {


    if ( argc < 2 ){
        opcoesInicializacao();
    }
    else{
        const unsigned char opt = argv[1][0];
        switch ( opt ){
            case '1':
                corAtualVarinha = AMARELO;
            break;
            case '2':
                corAtualVarinha = VERDE;
            break;
            case '3':
                corAtualVarinha = CIANO;
            break;
            case '4':
                corAtualVarinha = AZUL;
            break;
            case '5':
                corAtualVarinha = MAGENTA;
            break;
            case '6':
                corAtualVarinha = VERMELHO;
            break;
            default:
                opcoesInicializacao();
        }
    }

    if (inicializadores() == -1){
        return -1;
    }

    // Atualiza a tela
    al_flip_display();

    AllVariaveisJogo allVariaveisJogo;

    bool finalizarJogo = false,
         redraw        = false;

    int  contadorSegundos   = 0,
         contadorMinutos    = 0;

    funcaoVarinha = nenhuma;

    TempoFruto tempoFruto;
    reiniciaTempoParaFrutoMudar(&tempoFruto);

    telaAtual = telaMenu;
    PosicaoFruto posicaoFruto;

    Fruta *frutas;
    frutas = malloc(quantidadeFruto * sizeof(Fruta));
    reiniciarParaNovoJogo(frutas, &posicaoFruto, &allVariaveisJogo);

    CorCursor corCursor;

    al_start_timer(timer);
    al_start_timer(contador);
    al_set_target_bitmap(al_get_backbuffer(janela));
    bitmap = al_get_backbuffer(janela);

    //ALLEGRO_BITMAP *esquerda = al_create_sub_bitmap( bitmap, 0, 0, imageProc_getLargura(), imageProc_getAltura() );
    //ALLEGRO_BITMAP *direita = al_create_sub_bitmap( bitmap, imageProc_getLargura(), 0, imageProc_getLargura(), imageProc_getAltura() );

    imageProc_init( 0 ); // Função de inicialização
    imageProc_carregaCores();

    float x,y;
//printf("INICIANDO JOGO....\n");
    while (!finalizarJogo){
        //registrando evento
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        //fechando janela
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
             finalizarJogo = true;
        }

/* ----------------------------          ------------------------------------------------- */
/* ---------------------------- ANIMACAO ------------------------------------------------- */
/* ----------------------------          ------------------------------------------------- */

        else if(evento.type == ALLEGRO_EVENT_TIMER) {


            if (allVariaveisJogo.power <= 0){
                allVariaveisJogo.gameOver = true;
                telaAtual = telaFeedback;
            }

// --------------- TIME PARA ATUALIZACAO DOS FRUTOS ------------------------------------

            if (evento.timer.source == contador && telaAtual == telaJogo && !allVariaveisJogo.pausado){

                allVariaveisJogo.enableSimularArvore = false;

                if (posicaoFruto.index < MAX){
                    arvore_proximoPontoCrescimentoFrutas(&posicaoFruto.posicao[posicaoFruto.index].PosX, &posicaoFruto.posicao[posicaoFruto.index].PosY, &posicaoFruto.posicao[posicaoFruto.index].id);

                    if (posicaoFruto.posicao[posicaoFruto.index].id != -1){
                        posicaoFruto.index++;
                    }
                }


                if (allVariaveisJogo.aguardandoPoder){
                    allVariaveisJogo.tempoCursorNoBotaoPoder++;
                }

                contadorSegundos++;
                if (contadorSegundos >= 60){
                    contadorMinutos++;
                    contadorSegundos = 0;
                }

                if (allVariaveisJogo.contadorJogoMinuto == 0 && allVariaveisJogo.contadorJogoSegundo == 1){
                    allVariaveisJogo.gameOver = true;
                    telaAtual = telaFeedback;
                }

                allVariaveisJogo.contadorJogoSegundo -= 1;
                if (allVariaveisJogo.contadorJogoSegundo <= 0){
                    allVariaveisJogo.contadorJogoSegundo = 60;
                    if (allVariaveisJogo.contadorJogoMinuto > 0){
                        allVariaveisJogo.contadorJogoMinuto -= 1;
                    }
                }

                desenhandoNovoFruto(frutas, &allVariaveisJogo, &posicaoFruto);
                atualizandoPoderEstadoFruto(frutas, &allVariaveisJogo, &tempoFruto, &posicaoFruto);
                atualizandoFuncaoVarinha(&allVariaveisJogo);

            }

// --------------- TIME PARA REDRAW ------------------------------------

            if (evento.timer.source == timer){


                allVariaveisJogo.contadorFrame++;
                if (allVariaveisJogo.contadorFrame >= 60){
                    allVariaveisJogo.contadorFrame = 0;
                }

                if (allVariaveisJogo.enableSimularArvore){

                    if (allVariaveisJogo.contadorRegacaoArvore < 5){
                        allVariaveisJogo.contadorRegacaoArvore++;
                        arvore_simulaArvore(2);
                    }

                    else {
                        allVariaveisJogo.power -=50;
                    }
                    funcaoVarinha = nenhuma;
                    allVariaveisJogo.tempoFuncaoVarinha = 0;
                    allVariaveisJogo.enableSimularArvore = false;
                }

                else {
                    // if (allVariaveisJogo.contadorFrame % 5 == 0 ){
                    //     imageProc_atualizaXY( &allVariaveisJogo.mouse_x, &allVariaveisJogo.mouse_y, corAtualVarinha );
                    //     x = allVariaveisJogo.mouse_x;
                    //     y = allVariaveisJogo.mouse_y;
                    //     //printf("x: %f --- y: %f\n\n", (x / 320) * LARGURA_TELA, (y / 240) * ALTURA_TELA);
                    //
                    //     x = ((x / 320) * LARGURA_TELA);
                    //     y = ((y / 240) * ALTURA_TELA);
                    //
                    //     allVariaveisJogo.mouse_x = x;
                    //     allVariaveisJogo.mouse_y = y;
                    //
                    // }

                     if (allVariaveisJogo.contadorFrame % 20 == 0){

                         //imageProc_desenhaImagem( esquerda, direita );

                         //printf("x: %d - y: %d\n", allVariaveisJogo.mouse_x, allVariaveisJogo.mouse_y  );

                         if (!allVariaveisJogo.pausado){
                            arvore_simulaArvore(0);
                         }
                     }
                }

                for (int i = 0; i < quantidadeFruto; i++){
                    if (frutas[i].naTela){

                        if (frutas[i].estadoAtual == maduro){
                            if (frutas[i].r < 230){
                                frutas[i].r ++;
                            }
                            if (frutas[i].g > 0){
                                frutas[i].g --;
                            }
                            if (frutas[i].raio <= 15 ){
                                frutas[i].raio += 0.02;
                            }

                            if (frutas[i].pragaFruto){
                                if (frutas[i].tamanhoPraga < 8){
                                    frutas[i].tamanhoPraga += 0.01;
                                }
                            }
                        }

                        else if (frutas[i].estadoAtual == estragado){
                            if (frutas[i].g > 70) {
                                frutas[i].g --;
                                frutas[i].r = 80;
                            }
                            else if (frutas[i].r > 80){
                                frutas[i].r --;
                            }
                            if (frutas[i].raio >= 13){
                                frutas[i].raio -= 0.02;
                            }
                        }
                    }
                }

/* ----------------------------                 ------------------------------------------------- */
/* ----------------------------  VISAO COMPUT.  ------------------------------------------------- */
/* ----------------------------                 ------------------------------------------------- */

                //-----------TELA JOGO ------------

                if (telaAtual == telaJogo){

                    //printf("ENTROOOOOOOOO...\n");

                    al_hide_mouse_cursor(janela);

                    if (!allVariaveisJogo.pausado){

                        if (allVariaveisJogo.mouse_x > LARGURA_TELA * 0.90 &&
                            allVariaveisJogo.mouse_x < LARGURA_TELA * 1.00 &&
                            allVariaveisJogo.mouse_y > ALTURA_TELA  * 0.00 &&
                            allVariaveisJogo.mouse_y < ALTURA_TELA  * 0.10 ){

                                /* code */
                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                                allVariaveisJogo.pausado = true;
                        }

                        else if (allVariaveisJogo.mouse_x > ((LARGURA_TELA/5) * 1)-50 &&
                                allVariaveisJogo.mouse_x < ((LARGURA_TELA/5) * 1)+50 &&
                                allVariaveisJogo.mouse_y > ALTURA_TELA  * 0.90 &&
                                allVariaveisJogo.mouse_y < ALTURA_TELA && !allVariaveisJogo.disableFuncaoVarinha && funcaoVarinha == nenhuma){

                                /* code */
                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                                allVariaveisJogo.aguardandoPoder = true;
                                if (allVariaveisJogo.tempoCursorNoBotaoPoder > 1){
                                    funcaoVarinha = regar;
                                    //corAtualVarinha = CIANO;
                                    corCursor.r = 0;
                                    corCursor.g = 0;
                                    corCursor.b = 255;

                                    allVariaveisJogo.aguardandoPoder = false;
                                    allVariaveisJogo.tempoCursorNoBotaoPoder = 0;
                                }
                        }

                        else if (allVariaveisJogo.mouse_x > ((LARGURA_TELA/5) * 2)-50 &&
                                 allVariaveisJogo.mouse_x < ((LARGURA_TELA/5) * 2)+50 &&
                                 allVariaveisJogo.mouse_y > ALTURA_TELA  * 0.90 &&
                                 allVariaveisJogo.mouse_y < ALTURA_TELA && !allVariaveisJogo.disableFuncaoVarinha && funcaoVarinha == nenhuma ){

                                /* code */
                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                                allVariaveisJogo.aguardandoPoder = true;
                                if (allVariaveisJogo.tempoCursorNoBotaoPoder > 1){
                                     funcaoVarinha = matar;
                                    // corAtualVarinha = VERDE;
                                     corCursor.r = 255;
                                     corCursor.g = 0;
                                     corCursor.b = 0;

                                    allVariaveisJogo.aguardandoPoder = false;
                                    allVariaveisJogo.tempoCursorNoBotaoPoder = 0;
                                }

                        }

                        else if (allVariaveisJogo.mouse_x > ((LARGURA_TELA/5) * 3)-50 &&
                                 allVariaveisJogo.mouse_x < ((LARGURA_TELA/5) * 3)+50 &&
                                 allVariaveisJogo.mouse_y > ALTURA_TELA  * 0.90 &&
                                 allVariaveisJogo.mouse_y < ALTURA_TELA && !allVariaveisJogo.disableFuncaoVarinha && funcaoVarinha == nenhuma ){

                                /* code */
                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                                allVariaveisJogo.aguardandoPoder = true;
                                if (allVariaveisJogo.tempoCursorNoBotaoPoder > 1){
                                     funcaoVarinha = colher;
                                    // corAtualVarinha = AZUL;
                                     corCursor.r = 0;
                                     corCursor.g = 255;
                                     corCursor.b = 0;

                                    allVariaveisJogo.aguardandoPoder = false;
                                    allVariaveisJogo.tempoCursorNoBotaoPoder = 0;
                                }

                        }

                        else if (allVariaveisJogo.mouse_x > ((LARGURA_TELA/5) * 4)-50 &&
                                 allVariaveisJogo.mouse_x < ((LARGURA_TELA/5) * 4)+5 &&
                                 allVariaveisJogo.mouse_y > ALTURA_TELA  * 0.90 &&
                                 allVariaveisJogo.mouse_y < ALTURA_TELA && !allVariaveisJogo.disableFuncaoVarinha && funcaoVarinha == nenhuma ){

                                /* code */
                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                            //    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA MLK\n");

                                //allVariaveisJogo.aguardandoPoder = true;
                                //if (allVariaveisJogo.tempoCursorNoBotaoPoder >= 0){
                                     funcaoVarinha = regarArvore;
                                    // corAtualVarinha = MAGENTA;
                                     corCursor.r = 255;
                                     corCursor.g = 0;
                                     corCursor.b = 255;

                                    //allVariaveisJogo.aguardandoPoder = false;
                                   //allVariaveisJogo.tempoCursorNoBotaoPoder = 0;
                                //}

                        }

                        else if (funcaoVarinha != nenhuma){

                            for (int i = 0; i < quantidadeFruto; i++){
                                if ( frutas[i].naTela &&
                                     allVariaveisJogo.mouse_x > (frutas[i].posX - frutas[i].raio) &&
                                     allVariaveisJogo.mouse_x < (frutas[i].posX + frutas[i].raio) &&
                                     allVariaveisJogo.mouse_y > (frutas[i].posY - frutas[i].raio) &&
                                     allVariaveisJogo.mouse_y < (frutas[i].posY + frutas[i].raio) ) {

                                   frutas[i].poderAplicado = funcaoVarinha;

                                }
                            }
                        }


                        else {
                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                                corCursor.r = 0;
                                corCursor.g = 0;
                                corCursor.b = 0;
                                //corAtualVarinha = AZUL;
                                allVariaveisJogo.aguardandoPoder = false;
                                allVariaveisJogo.tempoCursorNoBotaoPoder = 0;

                        }

                        //al_draw_filled_rectangle((LARGURA_TELA / 2 - 40) - 20, 600 - 100 , (LARGURA_TELA / 2 - 40) + 20 , 600 , al_map_rgb(255  , 0  , 255));

                        if (funcaoVarinha == regarArvore &&
                            !allVariaveisJogo.enableSimularArvore&&
                             allVariaveisJogo.mouse_x > (LARGURA_TELA / 2 - 40) - 20 &&
                             allVariaveisJogo.mouse_x < (LARGURA_TELA / 2 - 40) + 20 &&
                             allVariaveisJogo.mouse_y > 600 - 100&&
                             allVariaveisJogo.mouse_y < 600){

                                 allVariaveisJogo.enableSimularArvore = true;
                        }

                    }

                    else { //jogo pausado

                        if (allVariaveisJogo.mouse_x > LARGURA_TELA * 0.40 &&
                            allVariaveisJogo.mouse_x < LARGURA_TELA * 0.60 &&
                            allVariaveisJogo.mouse_y > ALTURA_TELA  * 0.20 &&
                            allVariaveisJogo.mouse_y < ALTURA_TELA  * 0.30){

                            al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                        }

                        else if (allVariaveisJogo.mouse_x > LARGURA_TELA * 0.40 &&
                                 allVariaveisJogo.mouse_x < LARGURA_TELA * 0.60 &&
                                 allVariaveisJogo.mouse_y > ALTURA_TELA  * 0.40 &&
                                 allVariaveisJogo.mouse_y < ALTURA_TELA  * 0.50){

                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                        }

                        else if (allVariaveisJogo.mouse_x > LARGURA_TELA * 0.40 &&
                                 allVariaveisJogo.mouse_x < LARGURA_TELA * 0.60 &&
                                 allVariaveisJogo.mouse_y > ALTURA_TELA  * 0.60 &&
                                 allVariaveisJogo.mouse_y < ALTURA_TELA  * 0.70){

                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                        }
                        else {
                                al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                        }

                    }

                 }

                for (int i = 0; i < posicaoFruto.index; i++){
                    if (posicaoFruto.posicao[i].id != -1){
                        arvore_atualizaXYGalho( &posicaoFruto.posicao[i].PosX, &posicaoFruto.posicao[i].PosY, posicaoFruto.posicao[i].id);
                    }
                }

                for (int i = 0; i < quantidadeFruto; i++){
                    if (frutas[i].id != -1 && frutas[i].naTela == true){
                        arvore_atualizaXYGalho( &frutas[i].posX, &frutas[i].posY, frutas[i].id);
                    }
                }

                redraw = true;
            }
        }

/* ----------------------------               ------------------------------------------------- */
/* ---------------------------- POSICAO MOUSE ------------------------------------------------- */
/* ----------------------------               ------------------------------------------------- */

    // -------------- ATRIBUINDO POSICAO DO MOUSE PARA VARIAVEIS X Y,
                     // ATUALIZAR VARIVEIS COM POSICAO X Y DA VISAO   -------------------
                     //allVariaveisJogo.mouse_x = evento.mouse.x;
                     //allVariaveisJogo.mouse_y = evento.mouse.y;

        else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){

//-----------TELA MENU INICIAL ------------

            if (telaAtual == telaMenu){

                al_show_mouse_cursor(janela);

                if (evento.mouse.x > LARGURA_TELA * 0.40 &&
                    evento.mouse.x < LARGURA_TELA * 0.60 &&
                    evento.mouse.y > ALTURA_TELA  * 0.20 &&
                    evento.mouse.y < ALTURA_TELA  * 0.30){

                    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }

                else if (evento.mouse.x > LARGURA_TELA * 0.40 &&
                         evento.mouse.x < LARGURA_TELA * 0.60 &&
                         evento.mouse.y > ALTURA_TELA  * 0.60 &&
                         evento.mouse.y < ALTURA_TELA  * 0.70){

                        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }
                else {
                        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }
            }

//-----------TELA FEEDBACK POS JOGO ------------

            else if (telaAtual == telaFeedback){

                al_show_mouse_cursor(janela);

                /* code */
            }
        }

/* ----------------------------                 ------------------------------------------------- */
/* ---------------------------- CLIQUE DO MOUSE ------------------------------------------------- */
/* ----------------------------                 ------------------------------------------------- */

        else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

//-----------TELA MENU INICIAL ------------

            if (telaAtual == telaMenu){
                if (evento.mouse.x > LARGURA_TELA * 0.40 &&
                    evento.mouse.x < LARGURA_TELA * 0.60 &&
                    evento.mouse.y > ALTURA_TELA  * 0.20 &&
                    evento.mouse.y < ALTURA_TELA  * 0.30){

                    telaAtual = telaJogo;
                    if (allVariaveisJogo.pausado){
                        reiniciarParaNovoJogo(frutas, &posicaoFruto, &allVariaveisJogo);
                        allVariaveisJogo.pausado = false;
                    }
                }

                else if (evento.mouse.x > LARGURA_TELA * 0.40 &&
                         evento.mouse.x < LARGURA_TELA * 0.60 &&
                         evento.mouse.y > ALTURA_TELA  * 0.60 &&
                         evento.mouse.y < ALTURA_TELA  * 0.70){

                        finalizarJogo = true;
                }
            }

//-----------TELA JOGO ------------

            else if (telaAtual == telaJogo){

                if (allVariaveisJogo.pausado == false){

                    if (evento.mouse.x > LARGURA_TELA * 0.90 &&
                        evento.mouse.x < LARGURA_TELA * 1.00 &&
                        evento.mouse.y > ALTURA_TELA  * 0.00 &&
                        evento.mouse.y < ALTURA_TELA  * 0.10){

                            allVariaveisJogo.pausado = true;

                    }

                    else if (evento.mouse.x > LARGURA_TELA * 0.20 &&
                             evento.mouse.x < LARGURA_TELA * 0.30 &&
                             evento.mouse.y > ALTURA_TELA  * 0.90 &&
                             evento.mouse.y < ALTURA_TELA ){

                    }

                    else if (evento.mouse.x > LARGURA_TELA * 0.45 &&
                             evento.mouse.x < LARGURA_TELA * 0.55 &&
                             evento.mouse.y > ALTURA_TELA  * 0.90 &&
                             evento.mouse.y < ALTURA_TELA ){

                    }

                    else if (evento.mouse.x > LARGURA_TELA * 0.70 &&
                             evento.mouse.x < LARGURA_TELA * 0.80 &&
                             evento.mouse.y > ALTURA_TELA  * 0.90 &&
                             evento.mouse.y < ALTURA_TELA ){

                    }
                }

                else { //jogo pausado

                    if (evento.mouse.x > LARGURA_TELA * 0.40 &&
                        evento.mouse.x < LARGURA_TELA * 0.60 &&
                        evento.mouse.y > ALTURA_TELA  * 0.20 &&
                        evento.mouse.y < ALTURA_TELA  * 0.30){

                            telaAtual = telaJogo;
                            allVariaveisJogo.pausado = false;
                    }

                    else if (evento.mouse.x > LARGURA_TELA * 0.40 &&
                             evento.mouse.x < LARGURA_TELA * 0.60 &&
                             evento.mouse.y > ALTURA_TELA  * 0.40 &&
                             evento.mouse.y < ALTURA_TELA  * 0.50){

                                telaAtual = telaMenu;
                    }
                }
            }

//-----------TELA FEEDBACK POS JOGO ------------

            else if (telaAtual == telaFeedback){

                if ( evento.mouse.x > LARGURA_TELA * 0.20 &&
                     evento.mouse.x < LARGURA_TELA * 0.30 &&
                     evento.mouse.y > ALTURA_TELA  * 0.90 &&
                     evento.mouse.y < ALTURA_TELA ){

                    allVariaveisJogo.gameOver = false;
                    reiniciarParaNovoJogo(frutas, &posicaoFruto, &allVariaveisJogo);
                    telaAtual = telaMenu;

                }

                else if (evento.mouse.x > LARGURA_TELA * 0.70 &&
                         evento.mouse.x < LARGURA_TELA * 0.80 &&
                         evento.mouse.y > ALTURA_TELA  * 0.90 &&
                         evento.mouse.y < ALTURA_TELA ){

                    reiniciarParaNovoJogo(frutas, &posicaoFruto, &allVariaveisJogo);
                    telaAtual = telaJogo;
                }
            }
        }


/* ----------------------------              ------------------------------------------------- */
/* ---------------------------- REDESENHANDO ------------------------------------------------- */
/* ----------------------------              ------------------------------------------------- */

        if (redraw && al_is_event_queue_empty(fila_eventos)) {
            redraw = false;

//-----------TELA MENU INICIAL ------------

            if (telaAtual == telaMenu){

                //al_draw_filled_rectangle(LARGURA_TELA * 0.40, ALTURA_TELA  * 0.20 , LARGURA_TELA * 0.60 , ALTURA_TELA * 0.30, al_map_rgb(255, 0  , 0));
                if (allVariaveisJogo.pausado){
                    al_draw_text(fonte_80, al_map_rgb(100, 100, 100), LARGURA_TELA/2, ALTURA_TELA * 0.2 , ALLEGRO_ALIGN_CENTRE , "Novo Jogo");
                }
                else
                    al_draw_text(fonte_80, al_map_rgb(100, 100, 100), LARGURA_TELA/2, ALTURA_TELA * 0.2 , ALLEGRO_ALIGN_CENTRE , "Jogar");

                //al_draw_filled_rectangle(LARGURA_TELA * 0.40, ALTURA_TELA  * 0.60 , LARGURA_TELA * 0.60 , ALTURA_TELA * 0.70 , al_map_rgb(255, 0  , 0));
                al_draw_text(fonte_80, al_map_rgb(100, 100, 100), LARGURA_TELA/2, ALTURA_TELA * 0.6 , ALLEGRO_ALIGN_CENTRE , "Sair");
            }

//-----------TELA JOGO ------------

            else if (telaAtual == telaJogo){


                if (allVariaveisJogo.pausado == true){

                    al_draw_filled_rectangle(LARGURA_TELA * 0.40, ALTURA_TELA  * 0.20 , LARGURA_TELA * 0.60 , ALTURA_TELA * 0.30, al_map_rgb(255, 255, 255));
                    al_draw_text(fonte_80, al_map_rgb(100, 100, 100), LARGURA_TELA/2, ALTURA_TELA * 0.2 , ALLEGRO_ALIGN_CENTRE , "Voltar");

                    al_draw_filled_rectangle(LARGURA_TELA * 0.40, ALTURA_TELA  * 0.40 , LARGURA_TELA * 0.60 , ALTURA_TELA * 0.50, al_map_rgb(255, 255, 255));
                    al_draw_text(fonte_80, al_map_rgb(100, 100, 100), LARGURA_TELA/2, ALTURA_TELA * 0.4 , ALLEGRO_ALIGN_CENTRE , "Sair");
                }

                else {
                    //caixa de colisao com regar arvore
                    //al_draw_filled_rectangle((LARGURA_TELA / 2 - 40) - 20, 600 - 100 , (LARGURA_TELA / 2 - 40) + 20 , 600 , al_map_rgb(255  , 0  , 255));

                    arvore_desenha(bitmap);

                    for (int i = 0; i < quantidadeFruto; i++){
                        if (frutas[i].naTela == true){
                            switch(frutas[i].estadoAtual){
                                case verde:
                                    al_draw_filled_circle(frutas[i].posX, frutas[i].posY, frutas[i].raio, al_map_rgb(frutas[i].r, frutas[i].g, frutas[i].b));
                                     break;
                                case maduro:
                                    al_draw_filled_circle(frutas[i].posX, frutas[i].posY, frutas[i].raio, al_map_rgb(frutas[i].r, frutas[i].g, frutas[i].b));
                                    if (frutas[i].pragaFruto){
                                        al_draw_line(frutas[i].posX - frutas[i].tamanhoPraga, frutas[i].posY, frutas[i].posX + frutas[i].tamanhoPraga , frutas[i].posY, al_map_rgb(100, 255, 255), 4);
                                    }
                                    break;
                                case estragado:
                                    al_draw_filled_circle(frutas[i].posX, frutas[i].posY, frutas[i].raio, al_map_rgb(frutas[i].r, frutas[i].g, frutas[i].b));
                                    if (frutas[i].pragaFruto){
                                        al_draw_filled_circle(frutas[i].posX+7, frutas[i].posY+5, 2, al_map_rgb(255, 255, 255));
                                        al_draw_filled_circle(frutas[i].posX-5, frutas[i].posY+5, 1, al_map_rgb(255, 255, 255));
                                        al_draw_filled_circle(frutas[i].posX-5, frutas[i].posY-7, 3, al_map_rgb(255, 255, 255));
                                    }
                                    break;
                            }
                        }
                    }
                    //al_draw_filled_rectangle(LARGURA_TELA * 0.20, ALTURA_TELA  * 0.90 , LARGURA_TELA * 0.30 , ALTURA_TELA , al_map_rgb(255, 0  , 0));
                    //al_draw_filled_rectangle(LARGURA_TELA * 0.45, ALTURA_TELA  * 0.90 , LARGURA_TELA * 0.55 , ALTURA_TELA , al_map_rgb(0  , 255, 0));
                    //al_draw_filled_rectangle(LARGURA_TELA * 0.70, ALTURA_TELA  * 0.90 , LARGURA_TELA * 0.80 , ALTURA_TELA , al_map_rgb(0  , 0  , 255));

                    al_draw_filled_rectangle(0, 600 , LARGURA_TELA, ALTURA_TELA , al_map_rgb(20  , 80  , 10));

                    al_draw_filled_rectangle(10, 10, 260, 40, al_map_rgb(0, 0  , 255));
                    al_draw_filled_rectangle(10 + ( allVariaveisJogo.contadorRegacaoArvore * 50 ), 10, 260, 40, al_map_rgb(255, 0  , 0));


                    //POSICAO COLICAO PODER
                    //al_draw_filled_rectangle(((LARGURA_TELA/5) * 1)+50, ALTURA_TELA  * 0.90 , ((LARGURA_TELA/5) * 1)-50 , ALTURA_TELA , al_map_rgb(255, 0  , 0));
                    //al_draw_filled_rectangle(((LARGURA_TELA/5) * 2)+50, ALTURA_TELA  * 0.90 , ((LARGURA_TELA/5) * 2)-50 , ALTURA_TELA , al_map_rgb(0  , 255, 0));
                    //al_draw_filled_rectangle(((LARGURA_TELA/5) * 3)+50, ALTURA_TELA  * 0.90 , ((LARGURA_TELA/5) * 3)-50 , ALTURA_TELA , al_map_rgb(0  , 0  , 255));
                    //al_draw_filled_rectangle(((LARGURA_TELA/5) * 4)+50, ALTURA_TELA  * 0.90 , ((LARGURA_TELA/5) * 4)-50 , ALTURA_TELA , al_map_rgb(255  , 0  , 255));

                    al_draw_filled_circle(((LARGURA_TELA/5) * 1)-50 + (al_get_bitmap_width(nota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(nota)/2), 45, al_map_rgba(255, 0  , 0, 250));
                    al_draw_filled_circle(((LARGURA_TELA/5) * 2)-30 + (al_get_bitmap_width(fogo)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(fogo)/2), 45, al_map_rgba(0  , 255, 0, 250));
                    al_draw_filled_circle(((LARGURA_TELA/5) * 3)-20 + (al_get_bitmap_width(cesto)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(cesto)/2), 45, al_map_rgba(0  , 0  , 255, 150));
                    al_draw_filled_circle(((LARGURA_TELA/5) * 4)-20 + (al_get_bitmap_width(gota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(gota)/2), 45, al_map_rgba(255  , 0  , 255, 150));

                    al_draw_bitmap(nota, ((LARGURA_TELA/5) * 1)-50, ALTURA_TELA  * 0.90, 0);
                    al_draw_bitmap(fogo, ((LARGURA_TELA/5) * 2)-30, ALTURA_TELA  * 0.90, 0);
                    al_draw_bitmap(cesto,((LARGURA_TELA/5) * 3)-25, ALTURA_TELA  * 0.90, 0);
                    al_draw_bitmap(gota, ((LARGURA_TELA/5) * 4)-25, ALTURA_TELA  * 0.90, 0);
                    al_draw_textf(fonte_80, al_map_rgb(222, 0, 0), 10, 40, ALLEGRO_ALIGN_LEFT , "score: %d", allVariaveisJogo.score);
                    al_draw_textf(fonte_80, al_map_rgb(222, 0, 0), 10, 80, ALLEGRO_ALIGN_LEFT , "power: %d", allVariaveisJogo.power);

                    if (allVariaveisJogo.contadorJogoSegundo < 10){
                        al_draw_textf(fonte_80, al_map_rgb(222, 0, 0), 10, 120, ALLEGRO_ALIGN_LEFT , "tempo: %d:0%d",allVariaveisJogo.contadorJogoMinuto, allVariaveisJogo.contadorJogoSegundo);
                    } else {
                        al_draw_textf(fonte_80, al_map_rgb(222, 0, 0), 10, 120, ALLEGRO_ALIGN_LEFT , "tempo: %d:%d",allVariaveisJogo.contadorJogoMinuto, allVariaveisJogo.contadorJogoSegundo);
                    }
                }

                switch(funcaoVarinha){
                    case regarArvore:
                        al_draw_filled_circle(((LARGURA_TELA/5) * 1)-50 + (al_get_bitmap_width(nota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(nota)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 2)-30 + (al_get_bitmap_width(fogo)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(fogo)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 3)-20 + (al_get_bitmap_width(cesto)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(cesto)/2), 45, al_map_rgba(0,0,0, 125));
                        al_draw_bitmap(gotaCursor, allVariaveisJogo.mouse_x  - (al_get_bitmap_width(gotaCursor)/2), allVariaveisJogo.mouse_y - (al_get_bitmap_height(gotaCursor)/2), 0);
                        break;
                    case regar:
                        al_draw_filled_circle(((LARGURA_TELA/5) * 2)-30 + (al_get_bitmap_width(fogo)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(fogo)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 3)-20 + (al_get_bitmap_width(cesto)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(cesto)/2), 45, al_map_rgba(0,0,0, 125));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 4)-20 + (al_get_bitmap_width(gota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(gota)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_bitmap(notaCursor, allVariaveisJogo.mouse_x - (al_get_bitmap_width(notaCursor)/2), allVariaveisJogo.mouse_y - (al_get_bitmap_height(notaCursor)/2), 0);
                        break;
                    case matar:
                        al_draw_filled_circle(((LARGURA_TELA/5) * 1)-50 + (al_get_bitmap_width(nota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(nota)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 3)-20 + (al_get_bitmap_width(cesto)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(cesto)/2), 45, al_map_rgba(0,0,0, 125));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 4)-20 + (al_get_bitmap_width(gota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(gota)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_bitmap(fogoCursor, allVariaveisJogo.mouse_x - (al_get_bitmap_width(fogoCursor)/2), allVariaveisJogo.mouse_y - (al_get_bitmap_height(fogoCursor)/2), 0);
                        break;
                    case colher:
                        al_draw_filled_circle(((LARGURA_TELA/5) * 1)-50 + (al_get_bitmap_width(nota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(nota)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 2)-30 + (al_get_bitmap_width(fogo)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(fogo)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 4)-20 + (al_get_bitmap_width(gota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(gota)/2), 45, al_map_rgba  (0,0,0, 125));
                        al_draw_bitmap(cestoCursor, allVariaveisJogo.mouse_x - (al_get_bitmap_width(cestoCursor)/2) , allVariaveisJogo.mouse_y - (al_get_bitmap_height(cestoCursor)/2), 0);
                        break;
                    case nenhuma:
                        al_draw_filled_circle(allVariaveisJogo.mouse_x, allVariaveisJogo.mouse_y, 10, al_map_rgb(corCursor.r, corCursor.g, corCursor.b));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 1)-50 + (al_get_bitmap_width(nota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(nota)/2), 45, al_map_rgba  (0,0,0,0));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 2)-30 + (al_get_bitmap_width(fogo)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(fogo)/2), 45, al_map_rgba  (0,0,0,0));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 3)-20 + (al_get_bitmap_width(cesto)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(cesto)/2), 45, al_map_rgba(0,0,0,0));
                        al_draw_filled_circle(((LARGURA_TELA/5) * 4)-20 + (al_get_bitmap_width(gota)/2), ALTURA_TELA  * 0.90 + (al_get_bitmap_height(gota)/2), 45, al_map_rgba  (0,0,0,0));
                        break;

                }
                //al_draw_filled_circle(allVariaveisJogo.mouse_x, allVariaveisJogo.mouse_y, 10, al_map_rgb(corCursor.r, corCursor.g, corCursor.b));
            }

//-----------TELA FEEDBACK POS JOGO ------------

            else if (telaAtual == telaFeedback){

                //al_draw_filled_rectangle(LARGURA_TELA * 0.20, ALTURA_TELA  * 0.90 , LARGURA_TELA * 0.30 , ALTURA_TELA , al_map_rgb(255, 0  , 0));
                al_draw_textf(fonte_80, al_map_rgb(222, 0, 0), LARGURA_TELA * 0.20 + 40, ALTURA_TELA  * 0.90, ALLEGRO_ALIGN_LEFT , "Sair");

                //al_draw_filled_rectangle(LARGURA_TELA * 0.70, ALTURA_TELA  * 0.90 , LARGURA_TELA * 0.80 , ALTURA_TELA , al_map_rgb(0  , 0  , 255));
                al_draw_textf(fonte_80, al_map_rgb(222, 0, 0), LARGURA_TELA * 0.70, ALTURA_TELA  * 0.90, ALLEGRO_ALIGN_LEFT , "Novo Jogo");


                al_draw_textf(fonte_80, al_map_rgb(222, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE , "Quantidade de pontos: %d", allVariaveisJogo.score);
            }

//----------- atualizando...
//if (allVariaveisJogo.contadorFrame % 3 == 0 ){

//}
            al_flip_display();
            al_clear_to_color(al_map_rgb(160, 230, 210));

            // Calcula a posição do cursor do mouse
            imageProc_atualizaXY( &allVariaveisJogo.mouse_x, &allVariaveisJogo.mouse_y, corAtualVarinha );
            x = allVariaveisJogo.mouse_x;
            y = allVariaveisJogo.mouse_y;
            //printf("x: %f --- y: %f\n\n", (x / 320) * LARGURA_TELA, (y / 240) * ALTURA_TELA);

            x = ((x / 320) * LARGURA_TELA);
            y = ((y / 240) * ALTURA_TELA);

            allVariaveisJogo.mouse_x = x;
            allVariaveisJogo.mouse_y = y;
        }
    }

//  desalocando...
    destruir();
    free(frutas);
    return 0;
}
//----------------------------------------------------------------------------------------------
int inicializadores () {

    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    al_init_image_addon();

    // Inicialização do add-on para uso de fontes
    al_init_font_addon();

    al_install_keyboard();

    al_init_primitives_addon();

    // Criamos a nossa janela - dimensões de 640x480 px
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);

    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return -1;
    }

    // Configura o título da janela
    al_set_window_title(janela, "PlantGoshi");

    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        return -1;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        return -1;
    }

    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return -1;
    }

    if (!al_install_audio()){
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return -1;
    }

    if (!al_init_acodec_addon()){
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return -1;
    }

    if (!al_reserve_samples(1)){
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS );
    if (!timer){
        fprintf(stderr, "Falha ao alocar timer.\n");
        return -1;
    }

    contador = al_create_timer(1.0);
    if (!contador){
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return -1;
    }

    fila_eventos= al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao alocar fila de eventos.\n");
        return -1;
    }

    fonte_80 = al_load_font("fonts/fonte1.ttf", 40, 0);
    if (!fonte_80){
        fprintf(stderr, "Falha ao alocar fonte 80.\n");
        return -1;
    }

    gota  = al_load_bitmap("img/gota.png");
    if (!gota){
        fprintf(stderr, "Falha ao alocar gota.\n");
        return -1;
    }

    gotaCursor  = al_load_bitmap("img/gotaCursor.png");
    if (!gotaCursor){
        fprintf(stderr, "Falha ao alocar gotaCursor.\n");
        return -1;
    }

    fogo  = al_load_bitmap("img/fogo.png");
    if (!fogo){
        fprintf(stderr, "Falha ao alocar gota.\n");
        return -1;
    }

    fogoCursor  = al_load_bitmap("img/fogoCursor.png");
    if (!fogoCursor){
        fprintf(stderr, "Falha ao alocar gotaCursor.\n");
        return -1;
    }

    cesto  = al_load_bitmap("img/cesto.png");
    if (!cesto){
        fprintf(stderr, "Falha ao alocar gota.\n");
        return -1;
    }

    cestoCursor  = al_load_bitmap("img/cestoCursor.png");
    if (!cestoCursor){
        fprintf(stderr, "Falha ao alocar cestoCursor.\n");
        return -1;
    }

    nota  = al_load_bitmap("img/nota.png");
    if (!gota){
        fprintf(stderr, "Falha ao alocar nota.\n");
        return -1;
    }

    notaCursor  = al_load_bitmap("img/notaCursor.png");
    if (!notaCursor){
        fprintf(stderr, "Falha ao alocar notaCursor.\n");
        return -1;
    }

    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_timer_event_source(contador));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return 1;
}
//----------------------------------------------------------------------------------------------
void destruir () {

//    imageProc_finaliza();
//    arvore_destroi();
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_font(fonte_80);

}
//----------------------------------------------------------------------------------------------
void AtribuindoPosicaoFrutoNovo (Fruta frutas[], int i, PosicaoFruto *posicaoFruto ){
    int k = i;
    int aux = false;
    int j;

        while (k < MAX) {

            if (i != k){

                if (posicaoFruto->posicao[k].id != -1 && posicaoFruto->posicao[k].used == false){

                    frutas[i].posX = posicaoFruto->posicao[k].PosX;
                    frutas[i].posY = posicaoFruto->posicao[k].PosY;
                    frutas[i].id   = posicaoFruto->posicao[k].id;

                    for (j = 0; j < MAX; j++){
                        if (posicaoFruto->posicao[j].used == true &&
                            frutas[i].posX + 100 > posicaoFruto->posicao[j].PosX &&
                            frutas[i].posX - 100 < posicaoFruto->posicao[j].PosX &&
                            frutas[i].posY + 100 > posicaoFruto->posicao[j].PosY &&
                            frutas[i].posY - 100 < posicaoFruto->posicao[j].PosY ){

                            aux = false;
                        }
                        else {
                            aux = true;
                        }
                        if (aux == false){
                            break;
                        }
                    }
                    if (aux == true){
                        posicaoFruto->posicao[k].used = true;
                        return;
                    }
                }
            }
            k++;
        }

    frutas[i].posX = 0;
    frutas[i].posY = 0;
    frutas[i].id   = -1;
}
//----------------------------------------------------------------------------------------------
void tempPosicaoNovoNo (Fruta frutas[], int i, PosicaoFruto *posicaoFruto ) {

    AtribuindoPosicaoFrutoNovo (frutas, i, posicaoFruto);

    if (frutas[i].id != -1){
        frutas[i].pronta        = true;
        frutas[i].estadoAtual   = verde;
        frutas[i].tempoMudanca  = 0;
        frutas[i].mudar         = false;
        frutas[i].naTela        = false;
        frutas[i].poderAplicado = nenhuma;
        frutas[i].raio          = 10;
        frutas[i].pragaFruto    = false;
        frutas[i].regado        = false,
        frutas[i].r             = 0;
        frutas[i].g             = 255;
        frutas[i].b             = 0;
        frutas[i].tamanhoPraga  = 0;
    }
}
//----------------------------------------------------------------------------------------------
void desenhandoNovoFruto (Fruta frutas[], AllVariaveisJogo *allVariaveisJogo, PosicaoFruto *posicaoFruto){

    allVariaveisJogo->timeDesenharNovoFruto++;
    if (allVariaveisJogo->timeDesenharNovoFruto >= 5){
        allVariaveisJogo->timeDesenharNovoFruto = 0;

       for (int i = 0; i < quantidadeFruto; i++){
            if (frutas[i].naTela == false){
                for (int j = 0; j < MAX; j++){
                    if (frutas[i].id == posicaoFruto->posicao[j].id){
                        posicaoFruto->posicao[j].used = false;
                    }
                }

                tempPosicaoNovoNo(frutas, i, posicaoFruto);
            }

            if (frutas[i].pronta == true && frutas[i].naTela == false){
                frutas[i].naTela = true;
                return;
           }
       }
    }
}
//----------------------------------------------------------------------------------------------
void reiniciarParaNovoJogo(Fruta frutas[], PosicaoFruto *posicaoFruto, AllVariaveisJogo *allVariaveisJogo){

    arvore_inicializar( 0, 0, 1500, 1.0, 0.0, 1.5, 600, 600);
    allVariaveisJogo->pausado                 = false;
    allVariaveisJogo->disableFuncaoVarinha    = false;
    allVariaveisJogo->gameOver                = false;
    allVariaveisJogo->aguardandoPoder         = false;
    allVariaveisJogo->enableSimularArvore     = false;

    allVariaveisJogo->timeDesenharNovoFruto   = 0;

    //allVariaveisJogo.funcaoVarinha       = nenhuma;
    allVariaveisJogo->tempoFuncaoVarinha      = 0;

    allVariaveisJogo->score                   = 0;
    allVariaveisJogo->power                   = 500;
    allVariaveisJogo->mouse_x                 = 100;
    allVariaveisJogo->mouse_y                 = 100;
    allVariaveisJogo->tempoCursorNoBotaoPoder = 0;
    allVariaveisJogo->contadorFrame           = 0;
    allVariaveisJogo->contadorRegacaoArvore   = 0;
    allVariaveisJogo->contadorJogoMinuto      = 2;
    allVariaveisJogo->contadorJogoSegundo     = 0;

    for (int i = 0; i < quantidadeFruto; i++){
        frutas[i].pronta        = false;
        frutas[i].estadoAtual   = verde;
        frutas[i].tempoMudanca  = 0;
        frutas[i].mudar         = false;
        frutas[i].id            = -1;
        frutas[i].naTela        = false;
        frutas[i].poderAplicado = nenhuma;
        frutas[i].pragaFruto    = false,
        frutas[i].regado        = false,
        frutas[i].r             = 0,
        frutas[i].g             = 255,
        frutas[i].b             = 0,
        frutas[i].tamanhoPraga  = 0;
    }
    for (int i = 0; i < MAX; i++){
        posicaoFruto->posicao[i].PosX = 0;
        posicaoFruto->posicao[i].PosY = 0;
        posicaoFruto->posicao[i].used = false;
        posicaoFruto->posicao[i].id   = -1;
        posicaoFruto->index           = 0;
    }
}
//----------------------------------------------------------------------------------------------
void reiniciaTempoParaFrutoMudar(TempoFruto *tempoFruto){

    tempoFruto->paraRegar   = 15;
    tempoFruto->verde       = 20;
    tempoFruto->maduro      = 40;
    tempoFruto->paraPraga   = 30;
    tempoFruto->estragado   = 50;

}
//----------------------------------------------------------------------------------------------
void atualizandoPoderEstadoFruto (Fruta frutas[], AllVariaveisJogo *allVariaveisJogo, TempoFruto *tempoFruto, PosicaoFruto *posicaoFruto){

    for (int i = 0; i < quantidadeFruto; i++){
        if (frutas[i].naTela == true){
            frutas[i].tempoMudanca ++;

            if (frutas[i].poderAplicado == regar && frutas[i].estadoAtual == verde){
                frutas[i].regado = true;
                frutas[i].tempoMudanca += 3;
            }

            else if (frutas[i].poderAplicado == matar && frutas[i].estadoAtual == maduro){
                frutas[i].pragaFruto = false;
            }

            else if (frutas[i].poderAplicado == colher && frutas[i].estadoAtual == verde){
                frutas[i].naTela = false;
                allVariaveisJogo->power -= 50;
            }

            else if (frutas[i].poderAplicado == colher && frutas[i].estadoAtual == maduro){
                frutas[i].naTela = false;
                if (frutas[i].pragaFruto)
                    allVariaveisJogo->power -= 50;

                if (frutas[i].raio > 14)
                    allVariaveisJogo->score += 70;
                else {
                    allVariaveisJogo->score += 35;
                    allVariaveisJogo->power -= 50;
                }
            }
            else if (frutas[i].poderAplicado == colher && frutas[i].estadoAtual == estragado){
                frutas[i].naTela = false;
                allVariaveisJogo->power -= 100;
            }

            if (frutas[i].estadoAtual == maduro && frutas[i].tempoMudanca == tempoFruto->paraPraga){
                if (rand()%2)
                    frutas[i].pragaFruto = true;
            }

            if (!frutas[i].regado && frutas[i].estadoAtual == verde && frutas[i].tempoMudanca == tempoFruto->paraRegar){
                if ((rand()%3) == 1){
                    frutas[i].estadoAtual = estragado;
                    frutas[i].tempoMudanca = 11;
                    //   frutas[i].raio = 13;
                }
                else {
                    frutas[i].pragaFruto = true;
                }
            }

            if (frutas[i].estadoAtual == verde && frutas[i].tempoMudanca >= tempoFruto->verde){
                frutas[i].estadoAtual++;
                //frutas[i].tempoMudanca = 0;
                //  frutas[i].raio = 15;
            }

            else if (frutas[i].estadoAtual == maduro && frutas[i].tempoMudanca >= tempoFruto->maduro){
                frutas[i].estadoAtual++;
                frutas[i].tempoMudanca = 0;
                //  frutas[i].raio = 13;
            }

            else if (frutas[i].estadoAtual == estragado && frutas[i].tempoMudanca >= tempoFruto->estragado){
                allVariaveisJogo->power -= 50;
                frutas[i].naTela = false;
                frutas[i].tempoMudanca = 0;

            }
        }
    }
}
//----------------------------------------------------------------------------------------------
void atualizandoFuncaoVarinha (AllVariaveisJogo *allVariaveisJogo){

    if (funcaoVarinha != nenhuma){
        allVariaveisJogo->tempoFuncaoVarinha++;
        allVariaveisJogo->disableFuncaoVarinha = true;
        if (allVariaveisJogo->tempoFuncaoVarinha >= 0)
        {
            allVariaveisJogo->disableFuncaoVarinha = false;
        }
        if (allVariaveisJogo->tempoFuncaoVarinha >= 5){
            allVariaveisJogo->tempoFuncaoVarinha = 0;
            funcaoVarinha = nenhuma;
            //allVariaveisJogo.disableFuncaoVarinha = false;
        }
    }
}
