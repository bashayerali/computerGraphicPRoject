#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include <GL\glew.h>

#include <math.h>
#include <stdlib.h>
#include<time.h>
#include<stdio.h>
#include <GL/glut.h>
#include <GL\freeglut.h>
#include <iostream>

#include <Windows.h>
#include <mmsystem.h>

using namespace std;


GLint win_width = 700,
win_hight = 700;
int w1 = 0;
int h1 = 0;

float transValueX = 0.0;;
float transValueY = 0.0;;
float transValueZ = 11.0;;
float nearp;
float farp;
float i = 1;


float angleMoon = 0.0, angleEarth = 0.0;
GLfloat sx = 0.2, sy = 0.2, sz = 0.2;
int planet1;
GLfloat black[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat white[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat shiness[] = { 0,0,0,0.0 };
GLfloat Amb[] = { 0.1,0.1,0.1,1.0 };
GLfloat Dif[] = { 1.0,1.0,1.0,1.0 };
GLfloat Spec[] = { .50,.50,.50,.10 };
GLfloat Pos[] = { -60 + transValueX,transValueY,transValueZ - 11,0.0 };

void drawCircle() {
    float theta;
    for (int i = 0; i < 360; i++) {
        theta = i * 3.14 / 180;
        glVertex2f(250 + 150 * cos(theta), 250 + 150 * sin(theta));
    }
}



GLuint LoadTexture(const char* filename, int width, int height)
{
    GLuint texture;
    unsigned char* data;
    FILE* file;

    //The following code will read in our RAW file
    file = fopen(filename, "rb");

    if (file == NULL)
    {
        cout << "Unable to open the image file" << endl << "Program will exit :(" << endl;
        exit(0);
        return 0;
    }

    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);

    fclose(file);


    // reorder the image colors to RGB not BGR
    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

    }
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_BGRA_EXT, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    glGenTextures(1, &texture);            //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it's array

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


    free(data); //free the texture array

    if (glGetError() != GL_NO_ERROR)
        printf("GLError in genTexture()\n");

    return texture; //return whether it was successfull
}

void FreeCreatedTexture(GLuint texture)
{
    glDeleteTextures(1, &texture);
}


void initLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT7);
    glLightfv(GL_LIGHT7, GL_AMBIENT, Amb);
    glLightfv(GL_LIGHT7, GL_DIFFUSE, Dif);
    glLightfv(GL_LIGHT7, GL_SPECULAR, Spec);
}

void print_text(const char* text, int len, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    double* matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

static void play(void)
{
    glutPostRedisplay();
}

static
void key(unsigned char keyPressed, int x, int y) // key handling
{
    switch (keyPressed) {
    
    case 'x':
        transValueX += 0.1;
        break;
    case 'X':
        transValueX -= 0.1;
        break;
    case 'y':
        transValueY += 0.1;
        break;
    case'Y':
        transValueY -= 0.1;
        break;
    case 'n': 
        i = 0.8; nearp = 11.8;
        break;
    case 'o':
        i = 2; nearp = 15.2;
        break;
    case 'r':
        i = 1; nearp = 12.2;
        break;
    case 'q':
    case 27:
        exit(0);
        break;

    default:
        fprintf(stderr, "\nKeyboard commands:\n\n"
            "q, <esc> - Quit\n");
        break;

    }
}


void init()
{
    transValueX = 0.4;
    transValueY = -1.6;
    transValueZ = 11.0;
    nearp = 12.2;
    farp = 10;

    glMatrixMode(GL_PROJECTION);
    gluPerspective(65, (win_width / win_hight), 1.01, 4);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);
    glPointSize(1.0);
    glLineWidth(2.0);

}
void reshapeFunc(GLint new_width, GLint new_hight)
{
    glViewport(0, 0, new_width, new_hight);
    win_width = new_width;
    win_hight = new_hight;

}
void Display(void)
{

    GLuint texture = LoadTexture("Data\\Image\\sun.bmp", 256, 256);
    GLUquadric* quad;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushMatrix();
    glOrtho(-i, i, -i, i, nearp, farp);
    gluLookAt(transValueX, transValueY, transValueZ, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (win_width / win_hight), nearp, farp);
    
    glLoadIdentity();


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);


    glEnable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture);
    quad = gluNewQuadric();
    gluQuadricTexture(quad, 40);
    glColor3f(1, 1, 1);
    glScalef(sx, sy, sz);


    glLightfv(GL_LIGHT7, GL_POSITION, Pos);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
    gluSphere(quad, 1.5, 50, 50);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glScalef(0.2, 0.2, 0.2);

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    texture = LoadTexture("Data\\Image\\earth.bmp", 256, 256);
    glBindTexture(GL_TEXTURE_3D, texture);
    quad = gluNewQuadric();
    gluQuadricTexture(quad, 80);
    glRotatef(0, 0, 1.0, 0.5);
    glRotatef(angleEarth, 0.0, 1.0, -0.5);
    glTranslatef(4.5, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.23, 0.23, 0.23);

    gluSphere(quad, 2.5, 50, 50);
    glDisable(GL_TEXTURE_3D);
    glPushMatrix();
    
    glEnable(GL_TEXTURE_2D);
    texture = LoadTexture("Data\\Image\\moon.bmp", 256, 256);
    glBindTexture(GL_TEXTURE_2D, texture);
    quad = gluNewQuadric();
    gluQuadricTexture(quad, 1);

    glRotatef(angleMoon, 0.0, 0.1, 0.05);
    //if moon position back earth remove light
    if (angleMoon >= 0 && angleMoon <= 30 || angleMoon <= 360 && angleMoon >= 330)
        glLightfv(GL_LIGHT7, GL_POSITION, shiness);

    if (angleMoon >= 160 && angleMoon <= 180) {
        glRotatef(0.0, 0.0, 0.1, 0.05);
        glColor4f(0.0, 0.0, 0.0,0.8);
        glutSolidSphere(0.5, 50, 50);
        drawCircle();
    }

    glTranslatef(4, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.5, 0.5, 0.5);
    //    glutSolidSphere(1.5, 50, 50);



    gluSphere(quad, 1.5, 50, 50);
    glDisable(GL_TEXTURE_2D);
    FreeCreatedTexture(texture);
    glPopMatrix();//moon made
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();//earth made
    glPopMatrix();

    if ((angleMoon >= 0 && angleMoon <= 30) || (angleMoon >= 330 && angleMoon <= 360)) {
        std::string text;
        text = "Lunar eclipse";
        glColor3f(1.0, 1.0, 1.0);
        print_text(text.data(), text.size(), 350, 500);
    }
    if ((angleMoon >= 160 && angleMoon <= 180)) {
        std::string text;
        text = "Solar eclipse";
        glColor3f(1.0, 1.0, 1.0);
        print_text(text.data(), text.size(), 350, 500);
    }

    glFlush();
}

void update(int value) {

    if ((angleMoon >= 0 && angleMoon < 180))
    {
        sx -= 0.0003; sy -= 0.0003; sz -= 0.0003;
    }
    else { sx += 0.0003; sy += 0.0003; sz += 0.0003; }


    angleMoon += 1.0;
    if (angleMoon > 360) {
        angleMoon -= 360;
    }
    angleEarth += 0.7;
    if (angleEarth > 360) {
        angleEarth -= 360;
    }
    glutPostRedisplay();
    glutTimerFunc(35, update, 0);
}

int main(int argc, char** argv)
{
    PlaySound(TEXT("Data\\Music\\Spacesound.wav"), NULL, SND_ASYNC | SND_LOOP);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Lunar & Solar eclipse");
    
    initLighting();
    init();
    glutDisplayFunc(Display);
    
    glutTimerFunc(25, update, 0);
    glutIdleFunc(play);
    glutKeyboardFunc(key);
    glutMainLoop();
    return 0;
}