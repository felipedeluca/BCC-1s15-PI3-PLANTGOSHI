#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <opencv/highgui.h>
#include <allegro5/allegro.h>

#include "estrutura_cam.h"

camera *camera_inicializa(int i);
void camera_converte(camera *cam, IplImage *image);
void camera_finaliza(camera *cam);
void camera_atualiza(camera *cam);
void camera_copia(camera *cam, unsigned char ***matriz, ALLEGRO_BITMAP *bitmap);
void camera_libera_matriz(camera *cam, unsigned char ***matriz);
unsigned char ***camera_aloca_matriz(camera *cam);

#endif
