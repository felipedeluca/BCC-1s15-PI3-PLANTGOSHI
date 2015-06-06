#ifndef __ESTRUTURA_CAM_H__
#define __ESTRUTURA_CAM_H__

#include <opencv/highgui.h>

typedef struct {
  unsigned char ***quadro;
  int largura, altura;
  float escalaX, escalaY; // Escala para redimensionamento da tela
  CvCapture *capture;
} camera;

#endif
