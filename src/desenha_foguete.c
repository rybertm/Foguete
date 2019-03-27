#include "desenha_foguete.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/freeglut.h>

int animar = 0, sec = 0, n = 0, cont = 0; // n conta estagios, n = 3 ultimo estagio/fundo escuro
float ySpeed = 0.0f, offsetX = -0.3f, dyScale = 0.0f, dySize, angle = 0; //dySize guarda o tamanho do fogo\
																		 \na transição anterior para a animação\
																		 \ficar mais suave


char *r = "'r' - reseta a animacao.";
char *enter = "'ENTER' - comeca a animacao.";
char *esc = "'ESC' - finaliza o programa.";

void inicia_glut(int argc, char *argv[])
{
    srand((unsigned) time(0));

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glClearColor(0.0,0.0,0.0,0.0); //Especifica um cor para o fundo
}

void reshape(int width, int height)
{
    if (height == 0)
        height = 1;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width >= height)
    {
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    }
    else
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
}

void keyboard(unsigned char key, int xpos, int ypos)
{
    switch(key)
    {
    case 27:
        exit(0);
        break;
    case 10:
    case 13:	// \n - ENTER/RETURN
        animar = 1;
        glutIdleFunc(idle);
        if(!cont) // Evita que a velocidade de animacao aumente ao pressionar ENTER enquanto a animacao esta ocorrendo
            glutTimerFunc(0,timer,1);
        break;
    case 'r':
        ySpeed = 0.0f;
        animar = 0;
        sec = 0;
        n = 0;
        cont = 0;
        offsetX = -0.3f;
        dyScale = 0.0f;
        angle = 0.0f;
        glutIdleFunc(NULL); //Passing NULL to glutIdleFunc disables the generation of the idle callback. - Menos computacao
        glutDisplayFunc(display_cena);
        glutPostRedisplay();
        break;
    }
}

void idle(void)
{
    if(animar)
    {
        if(dyScale < 1.4)
            dyScale += 0.005f;
        else
            dyScale = 0.7f;

        if(cont > 3)
        {
            ySpeed -= 0.005f;
            // Cria novo cenario com as nuven a cada 8 sec, 2 vezes
            if(n <= 3 && (sec % 8 == 0))
            {
                ySpeed = 0.0f;
                dyScale = 0.005f;
                glutDisplayFunc(display_lanca);
            }
            if(n == 2)
                angle += 0.3f;
            // Depois do terceiro cenario ele para as animacoes e computacoes
            if(n > 3)
            {
                animar = 0;
                glutIdleFunc(NULL);
            }
        }
        glutPostRedisplay();
    }
}

void timer(int value)
{
    if(animar)
    {
        cont++;
        if(cont > 3)
        {
            sec++;
            if(sec % 8 == 0)
            {
                n++;
                printf("\nn = %d\n", n);
                offsetX += 0.3; // Variavel para mudar a posicao das proximas nuvens no cenario
            }
        }
        glutTimerFunc(1000,timer,1);
    }
}

void desenha_cena(int width, int height)
{
    int x, y;

    x = (glutGet(GLUT_SCREEN_WIDTH) - 640) / 2;
    y = (glutGet(GLUT_SCREEN_HEIGHT) - 480) / 2;

    glutInitWindowPosition(x, y - 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Foguete");

    glutDisplayFunc(display_cena);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
}

void display_cena(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    display_fundo();

    display_bico(0.2f, 0.35f);
    display_base(0.2f, 0.0f);
    display_prop(0.2f, 0.0f);

    display_asaE(0.075f, -0.5f);
    display_asaD(-0.325f, -0.5f);

    glColor3f(0.0f, 0.0f, 0.0f);
    display_texto(-0.99f, 0.95f, enter);
    display_texto(-0.99f, 0.88f, r);
    display_texto(-0.99f, 0.81f, esc);

    display_fumaca();
    display_fogo(0.2f, -0.6f, 0.0 + dyScale, 0.1f);

    if(cont < 4)
        display_cont(0.875f, 0.35f);

    glutSwapBuffers();
}

void display_fundo()
{
    //	Céu
    glBegin(GL_QUADS);
    {
        glColor3ub(135, 210, 255);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    }
    glEnd();

    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f + ySpeed, 0.0f);

        glBegin(GL_QUADS);
        {
            glColor3ub(135, 206, 235);
            glVertex2f(-1.0f, 0.6f);
            glVertex2f(1.0f, 0.6f);
            glVertex2f(1.0f, -1.0f);
            glVertex2f(-1.0f, -1.0f);
        }
        glEnd();

        glBegin(GL_QUADS);
        {
            glColor3ub(130, 206, 215);
            glVertex2f(-1.0f, 0.2f);
            glVertex2f(1.0f, 0.2f);
            glVertex2f(1.0f, -1.0f);
            glVertex2f(-1.0f, -1.0f);
        }
        glEnd();
        //	Fim: Céu

        //	Chão
        glBegin(GL_QUADS);
        {
            glColor3ub(81, 88, 81);
            glVertex2f(-1.0f, -0.55f);
            glVertex2f(1.0f, -0.55f);
            glVertex2f(1.0f, -1.0f);
            glVertex2f(-1.0f, -1.0f);
        }
        glEnd();
        //	Fim: Chão
    }
    glPopMatrix();

    //	Árvores
    int i;
    float x = 0.9, y = -0.55;

    display_arv(x, y + ySpeed);

    x -= 0.2;
    y -= 0.2;

    display_arv(x, y + ySpeed);

    x = -0.85;
    y = -0.65;

    for (i = 0; i < 2; i++, x += 0.3, y += 0.1)
    {
        display_arv(x, y + ySpeed);
    }
    //	Fim : Árvores

    display_suporte(0.0f,0.0f + ySpeed);

    //	Nuvens
    display_nuvem(-0.75, 0.85 + ySpeed, 1.0);
    display_nuvem(0.45, 0.6 + ySpeed, 1.45);
    display_nuvem(-0.25, 0.5 + ySpeed, 1.3);
    //	Fim: Nuvens
}

void display_bico(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    //	Bico
    glBegin(GL_TRIANGLES);
    {
        glColor3ub(255, 165, 0);
        glVertex2f(-0.125f, 0.0f);
        glColor3ub(125, 125, 15);
        glVertex2f(0.125f, 0.0f);
        glColor3ub(255, 165, 0);
        glVertex2f(0.0f, 0.35f);
    }
    glEnd();
    //	Fim: Bico

    glPopMatrix();
}

void display_base(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    //	Base
    glBegin(GL_QUADS);
    {
        glColor3ub(125, 125, 125);
        glVertex2f(-0.125f, 0.35f);
        glVertex2f(0.125f, 0.35f);
        glColor3ub(255, 255, 255);
        glVertex2f(0.125f, -0.5f);
        glColor3ub(0, 0, 0);
        glVertex2f(-0.125f, -0.5f);
    }
    glEnd();
    //	Fim: Base

    //	Janela
    float raio = 0.057, posx, posy;
    int i;

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(255, 255, 51);
    int n = 100;
    for (i = 0; i <= n; i++)
    {
        float ang = 2 * PI * i / n;

        posx = raio * cos(ang);
        posy = raio * sin(ang);

        glVertex2f(posx, posy + 0.15f);
        glColor3ub(255, 255, 102);
    }
    glEnd();
    //	Fim: Janela

    glPopMatrix();
}

void display_prop(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    //	Propulsor
    glBegin(GL_QUADS);
    {
        glColor3ub(255, 255, 51);
        glVertex2f(-0.125f, -0.5f);
        glColor3ub(255, 255, 102);
        glVertex2f(0.125f, -0.5f);
        glColor3ub(255, 255, 153);
        glVertex2f(0.025f, -0.6f);
        glColor3ub(255, 255, 51);
        glVertex2f(-0.025f, -0.6f);
    }
    glEnd();
    //	Fim: Propulsor

    glPopMatrix();
}

void display_asaE(float x, float y)
{
    glPushMatrix();

    if(n < 2)
        glTranslatef(x, y, 0.0f);
    else if(n == 2)
        glTranslatef(x + (ySpeed/5), y + (ySpeed/2), 0.0f);
	//Pós desacoplamento, asas nao sao mais renderizadas
    else
    {
        glPopMatrix();
        return ;
    }

    glRotatef(0.0 + angle,0.0,0.0,1.0);

    //	AsaE
    glBegin(GL_TRIANGLES);
    {
        glColor3ub(255, 255, 51);
        glVertex2f(0.0f, 0.0f);
        glColor3ub(255, 255, 102);
        glVertex2f(0.0f, 0.3f);
        glColor3ub(255, 255, 153);
        glVertex2f(-0.225f, 0.0f);
    }
    glEnd();
    //	Fim: AsaE

    glPopMatrix();
}

void display_asaD(float x, float y)
{
    glPushMatrix();

    //	AsaD
    glScalef(-1.0f, 1.0f, 1.0f);
    display_asaE(x, y);
    //	Fim: AsaD

    glPopMatrix();
}

void display_arv(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    //	Arv
    glBegin(GL_QUADS);
    {
        glColor3ub(166, 128, 100);
        glVertex2f(-0.05f, 0.125f);
        glColor3ub(83, 49, 24);
        glVertex2f(0.0f, 0.125f);
        glVertex2f(0.0, -0.125f);
        glVertex2f(-0.05f, -0.125f);
    }
    glEnd();
    //	Fim: Arv

    glPopMatrix();

    display_folhagem(x - 0.07f, y + 0.12f);
}

void display_folhagem(float x, float y)
{
    float raio = 0.05, posx, xOffset = 0.0, posy, yOffset = 0.0;
    int i, j = 1;

    while (j < 4)
    {
        glPushMatrix();
        glTranslatef(x + xOffset, y + yOffset, 0.0f);

        glBegin(GL_TRIANGLE_FAN);
        {
            glColor3ub(54, 130, 10);
            for (i = 0; i < 1000; i++)
            {
                float ang = 2.0 * PI * i / 1000;

                posx = raio * cos(ang);
                posy = raio * sin(ang);

                glVertex2f(posx, posy);
                glColor3ub(77, 168, 59);
            }
        }
        glEnd();

        glPopMatrix();

        xOffset += 0.045;
        yOffset += 0.065;
        j++;

        // Retorna terceiro circulo a altura do primeiro
        if (j % 2 != 0)
            yOffset -= 0.13;
    }
}

void display_nuvem(float x, float y, float scale)
{
    float raio = 0.05f, posx, xOffset = 0.0f, posy;
    int i, j = 0;

    while (j < 3)
    {
        glPushMatrix();
        glScalef(scale, scale, 1.0);
        glTranslatef(x + xOffset, y, 0.0f);

        glBegin(GL_TRIANGLE_FAN);
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            int n = 100;
            for (i = 0; i <= n; i++)
            {
                float ang = 2.0 * PI * i / n;

                posx = raio * cos(ang);
                posy = raio * sin(ang);

                glVertex2f(posx, posy);
            }
        }
        glEnd();

        glPopMatrix();

        xOffset += 0.07;
        j++;
    }
}

void display_suporte(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    // Suporte maior
    glBegin(GL_QUADS);
    {
        glColor3ub(255, 255, 255);
        glVertex2f(-0.45f, 0.25f);
        glColor3ub(188, 198, 204);
        glVertex2f(-0.40f, 0.25f);
        glVertex2f(-0.40f, -0.65f);
        glColor3ub(0, 0, 0);
        glVertex2f(-0.45f, -0.65f);
    }
    glEnd();

    glBegin(GL_QUADS);
    {
        glColor3ub(188, 198, 204);
        glVertex2f(-0.45f, 0.25f);
        glColor3ub(255, 255, 255);
        glVertex2f(0.1f, 0.25f);
        glVertex2f(0.1f, 0.2f);
        glColor3ub(0, 0, 0);
        glVertex2f(-0.45f, 0.2f);
    }
    glEnd();
    //	Fim : Suporte maior

    //	Suporte menor
    glBegin(GL_QUADS);
    {
        glColor3ub(255, 255, 255);
        glVertex2f(-0.25f, -0.05f);
        glColor3ub(188, 198, 204);
        glVertex2f(-0.20f, -0.05f);
        glVertex2f(-0.20f, -0.65f);
        glColor3ub(0, 0, 0);
        glVertex2f(-0.25f, -0.65f);
    }
    glEnd();

    glBegin(GL_QUADS);
    {
        glColor3ub(188, 198, 204);
        glVertex2f(-0.25f, -0.05f);
        glColor3ub(255, 255, 255);
        glVertex2f(0.1f, -0.05f);
        glVertex2f(0.1f, -0.1f);
        glColor3ub(0, 0, 0);
        glVertex2f(-0.25f, -0.1f);
    }
    glEnd();

    glPopMatrix();
    //	Fim: Suporte menor
}

void display_texto(float x, float y, char* texto)
{
    char *c;

    glRasterPos2f(x, y);

    for (c = texto; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void display_lanca(void)
{
    //	Céu Escuro
    glBegin(GL_QUADS);
    {
        glColor3ub(0, 0, 0);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    }
    glEnd();
    //	Fim: Céu Escuro

    glPushMatrix();

    if(n > 2)
        glTranslatef(0.0, 0.0 + ySpeed, 0.0);
    {
        //	Céu
        glBegin(GL_QUADS);
        {
            glColor3ub(135, 210, 255);
            glVertex2f(-1.0f, 1.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(1.0f, -1.0f);
            glVertex2f(-1.0f, -1.0f);
        }
        glEnd();
        //	Fim: Céu
    }
    glPopMatrix();


    display_bico(0.2f, 0.35f);
    display_base(0.2f, 0.0f);
    display_prop(0.2f, 0.0f);

    display_asaE(0.075f, -0.5f);
    display_asaD(-0.325f, -0.5f);

    glColor3f(0.0f, 0.0f, 0.0f);
    display_texto(-0.99f, 0.95f, enter);
    display_texto(-0.99f, 0.88f, r);
    display_texto(-0.99f, 0.81f, esc);

    if(n != 3)
    {
        display_nuvem(0.67 - offsetX, 0.95 + ySpeed, 1.15);
        display_nuvem(-0.55 + offsetX, 0.9 + ySpeed, 1.35);
        display_nuvem(-0.1 - offsetX, 0.9 + ySpeed, 1.7);
    }

    display_fumaca(0.0f, 0.0f);
    display_fogo(0.2f, -0.6f, dySize + dyScale, 0.1f);

    glutSwapBuffers();
}

void display_fogo(float x, float y, float scale, float height)
{
    if(sec % 8 == 0)
        dySize = scale;

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(1.0, scale, 1.0);

    glBegin(GL_TRIANGLE_FAN);
    {
        glColor3ub(226, 88, 34);
        glVertex2f(-0.025f, 0.0f);
        glVertex2f(0.025f, 0.0f);
        glColor3ub(185, 69, 24);
        glVertex2f(0.05f, -height);
        glColor3ub(226, 88, 34);
        glVertex2f(0.0125f, -height +0.05f);
        glVertex2f(0.0f, -height);
        glColor3ub(229, 105, 57);
        glVertex2f(-0.0125f, -height + 0.05f);
        glColor3ub(226, 88, 34);
        glVertex2f(-0.05f, -height);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    {
        glVertex2f(-0.025f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-0.0125f, -height);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    {
        glVertex2f(0.025f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0125f, -height);
    }
    glEnd();

    glPopMatrix();
}

void display_fumaca()
{
    // obs: ySpeed é negativo
    float raio = 0.0f - (ySpeed / 10), posx, xOffset, yOffset, posy;
    int i, j = 0;

    xOffset = rand_float(-0.2f, 0.6f);
    yOffset = rand_float(-0.7f, -1.0f);

    if((raio >= 0.07) || (sec >= 8))
        raio = 0.07;

    for(j = 0; j < 100; j++)
    {
        glPushMatrix();
        {
            glTranslatef(xOffset, yOffset, 0.0f);

            glBegin(GL_TRIANGLE_FAN);
            {
                glColor3ub(125, 125, 125);
                int n = 100;
                for (i = 0; i <= n; i++)
                {
                    float ang = 2.0 * PI * i / n;

                    posx = raio * cos(ang);
                    posy = raio * sin(ang);

                    glVertex2f(posx, posy);
                }
            }
            glEnd();
        }
        glPopMatrix();

        xOffset = rand_float(-0.2f, 0.6f);
        yOffset = rand_float(-0.7f, -1.0f);
    }

}

void display_cont(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glColor3d(0, 0, 0);
    glBegin(GL_QUADS);
    {
        glVertex2f(0.125f, 0.125f);
        glVertex2f(0.125f, -0.125f);
        glVertex2f(-0.125f, -0.125f);
        glVertex2f(-0.125f, 0.125f);
    }
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    switch(cont)
    {
    case 0:
    case 1: // Desenha num 3
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.1f, 0.0f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.09f, 0.05f, 0.0f);
            glScalef(1.0f, 0.5f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.09f, -0.05f, 0.0f);
            glScalef(1.0f, 0.5f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.0f, -0.1f, 0.0f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();
        break;
    case 2: // Desenha num 2
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.1f, 0.0f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.09f, 0.05f, 0.0f);
            glScalef(1.0f, 0.5f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.09f, -0.05f, 0.0f);
            glScalef(1.0f, 0.5f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.0f, -0.1f, 0.0f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(-0.01f, 0.1f);
                glVertex2f(0.01f, 0.1f);
                glVertex2f(0.01f, -0.1f);
                glVertex2f(-0.01f, -0.1f);
            }
            glEnd();
        }
        glPopMatrix();
        break;
    case 3: // Desenha num 1
        glBegin(GL_QUADS);
        {
            glVertex2f(-0.01f, 0.1f);
            glVertex2f(0.01f, 0.1f);
            glVertex2f(0.01f, -0.1f);
            glVertex2f(-0.01f, -0.1f);
        }
        glEnd();
        break;
    }
}

// Float aleatorio [min,max]
float rand_float(float min, float max)
{

    return min + ( ((float)rand()/(float)(RAND_MAX)) * (max - min) );
}
