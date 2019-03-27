#include <stdio.h>
#include <GL/freeglut.h>

#define PI 3.141592f

void inicia_glut(int argc, char *argv[]);
void reshape(int width, int height);
void keyboard(unsigned char key, int xpos, int ypos);
void idle(void);
void timer(int value);
void desenha_cena(int width, int height);
void display_cena(void);
void display_fundo(void);
void display_bico(float x, float y);
void display_base(float x, float y);
void display_prop(float x, float y);
void display_asaE(float x, float y);
void display_asaD(float x, float y);
void display_arv(float x, float y);
void display_folhagem(float x, float y);
void display_nuvem(float x, float y, float scale);
void display_suporte(float x, float y);
void display_texto(float x, float y, char* texto);
void display_lanca(void);
void display_fogo(float x, float y, float scale, float height);
void display_fumaca();
void display_cont(float x, float y);
float rand_float(float min, float max);
