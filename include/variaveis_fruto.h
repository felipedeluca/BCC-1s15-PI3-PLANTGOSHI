#ifndef __VARIAVEIS_FRUTO_H__
#define __VARIAVEIS_FRUTO_H__


enum tala_atual_jogo {telaMenu, 
                      telaJogo, 
                      telaFeedback
} telaAtual;

enum estado_do_fruto {verde, 
                      maduro, 
                      estragado,
                      comido
} estadoAtual;

enum funcao_da_varinha {regarArvore,
                        regar, 
                        matar, 
                        colher,
                        nenhuma
} funcaoVarinha;

typedef struct {

    int PosX,
        PosY,
        used,
        id;

}Posicao;

typedef struct {

    int index;
    Posicao posicao[MAX];

}PosicaoFruto;

typedef struct {        

    int posX,
        posY,
        tempoMudanca, 
        estadoAtual,
        poderAplicado,
        id,
        r, g, b;

    float raio,
          tamanhoPraga;

    bool mudar,
         pronta,
         naTela,
         pragaFruto,
         regado;

} Fruta;

typedef struct {

    int verde,
        maduro,
        estragado,
        paraPraga,
        paraRegar;

}TempoFruto;

typedef struct {

    int r,
        g,
        b;

}CorCursor;

typedef struct {
    
    bool pausado,
         disableFuncaoVarinha,
         gameOver,
         aguardandoPoder,
         enableSimularArvore;

    int timeDesenharNovoFruto,

        //funcaoVarinha,
        tempoFuncaoVarinha,

        score,
        power,
        mouse_x,
        mouse_y,
        tempoCursorNoBotaoPoder,
        contadorFrame,
        contadorRegacaoArvore,
        contadorJogoMinuto,
        contadorJogoSegundo;

}AllVariaveisJogo;

#endif