#include "desenha_foguete.h"

#include <stdio.h>
#include <GL/freeglut.h>

int main(int argc, char *argv[])
{
	inicia_glut(argc, argv);
	desenha_cena(600, 600);

	glutMainLoop();
	return 0;
}
