#include<windows.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include <iostream>
#include <math.h>

const double PI = 3.14159265359;

using namespace std;
void displayCallback();
void reshape(int, int);
void timer(int);
void drawGrid();
void keyboard(unsigned char, int, int);

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(200,100);
    glutInitWindowSize(500,500);

    glutCreateWindow("Window 1");
    glutDisplayFunc(displayCallback);
    // window reshape callback
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
}

int offsetX = -10;
int offsetY = -10;
float playerX = 1.45f;
float playerY = 2.37f;
double playerAngle = 45.0;
double rays_t[] = {0}; // rays length, for simplicity lets cast 1 ray so the we don't fuck up

const int worldMap[20][20] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

void castRays();

void keyboard(unsigned char key, int x, int y) {
    float deltaY = 0.0f;
    float deltaX = 0.0f;

    switch(key) {
        case 'w':
            deltaY= 0.1f;
            break;
        case 'a':
            deltaX= -0.1f;
            break;
        case 's':
            deltaY= -0.1f;
            break;
        case 'd':
            deltaX= 0.1f;
            break;
        case 'j': //steering CCW
            if (playerAngle + 1.f >= 360.0f) {
                playerAngle = playerAngle - 360.f;
            }
            playerAngle += 1.f;
            break;
        case 'k': //steering CW
            if (playerAngle - 1.f < 0.0f) {
                playerAngle = 360.0f - playerAngle;
            }
            playerAngle -= 1.f;
            break;
    }

    //hit detection
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++) {
            int cell = worldMap[i][j];
            if (cell == 1) {
                float minX = j+offsetX;
                float maxX = j+offsetX+1;
                float minY = -1*(i+offsetY) - 1;
                float maxY = (-1*(i+offsetY)) ;
                // player + delta is in some wall block: so we dont walk
                if ((playerX+deltaX > minX && playerX+deltaX < maxX) && (playerY+deltaY > minY && playerY+deltaY < maxY)) {
                    cout << playerX << " " << playerY << "\n";
                    cout << "stuck: " << minX << " " << minY << " " << maxX << " " << maxY << " " << "\n";
                    return;
                }
            }
         }
    cout << playerX << " " << playerY << " " << playerAngle << "\n";
    playerX += deltaX;
    playerY += deltaY;
}

void drawGrid() {
    for (int i = -10; i <= 10; i++) {
        glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(i, -10);
        glVertex2f(i, 10);
        glVertex2f(-10, i);
        glVertex2f(10, i);
        glEnd();
    }
}

void drawMap() {
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++) {
            int cell = worldMap[i][j];
            if (cell == 1) { // Wall
                glBegin(GL_POLYGON);
                glColor3f(1.0, 1.0, 1.0);
                // std::cout << j+offsetX << " " << -1*i+offsetY << "\n";
                glVertex2f(j+offsetX, -1*(i+offsetY));
                glVertex2f(j+offsetX, (-1*(i+offsetY)) - 1);
                glVertex2f(j+offsetX+1, (-1*(i+offsetY)) - 1);
                glVertex2f(j+offsetX+1, (-1*(i+offsetY)));
                glEnd();
            }
            
        }
}

void drawPlayer() {
    glTranslatef(playerX, playerY, 0.0f);
    glRotatef(playerAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0.5, 0);
    glVertex2f(-0.5, 0.3);
    glVertex2f(-0.5, -0.3);
    glEnd();
}

int isAtQuadrant(float deg) {
    //check which quadrant doeas the ray in (ref to the ray origin)
    if (deg >= 0.0f && deg < 90.0f) return 0;
    if (deg >= 90.0f && deg < 180.0f) return 1;
    if (deg >= 180.0f && deg < 270.0f) return 2;
    if (deg >= 270.0f && deg < 360.0f) return 3;
    else throw exception();
}

void castRays() {
    
    //DDA Raycaster naive implementation

    // transform player from viewport to world coord
    float pX = playerX - offsetX;
    float pY = playerY - offsetY;
    
    // check quadrant of current player angle
    int quadrant = isAtQuadrant(playerAngle);
    float deg = playerAngle - (((float) quadrant)*90.0f);
    float lx = 0;
    float ly = 0;
    float xlen = 0;
    float ylen = 0;

    float ax = floor(pX); // grid origin that player is currently in.
    float ay = floor(pY);
    float vx = 1.0f; // direction to step lx 
    float vy = 1.0f; // direction to step ly
    // cout << "quadrant " << quadrant << "\n";
    switch(quadrant) {
        case 0: xlen = 1.0f - (pX - ax);
                ylen = (pY - ay); 
                break;
        case 1: xlen = (pX - ax);
                ylen = (pY - ay); 
                vx = -1.0f;
                break;
        case 2: xlen = (pX - ax);
                ylen = 1.0f - (pY - ay); 
                vx = -1.0f;
                vy = -1.0f;
                break;
        case 3: xlen = 1.0f - (pX - ax);
                ylen = 1.0f - (pY - ay); 
                vy = -1.0f;
                break;
    }

    int count = 0;
    lx = xlen/cos((deg*PI)/180.f);
    ly = ylen/sin((playerAngle*PI)/180.f);
    cout <<"xlen, ylen "<< xlen << " " << ylen << "\n";
    cout <<"cos, sin "<< cos((deg*PI)/180.f) << " " << sin((deg*PI)/180.f) << "\n";
    while (count < 1) {
        if (lx <= ly) {
            xlen += 1;
            lx = xlen/cos((deg*PI)/180.f);
        }
        else if (ly < lx) {
            ylen += 1;
            ly = ylen/sin((deg*PI)/180.f);
        }
        count++;
    }

    float maxL = max(lx , ly);

    cout <<"xlen', ylen' "<< xlen << " " << ylen << "\n";
    cout <<"LxLy"<< lx << " " << ly << "\n";

    //draw ray
    glLoadIdentity();
    glTranslatef(playerX, playerY, 0.0f);
    glRotatef(playerAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    float uVecX = cos((deg*PI)/180.f);
    float uVecY = sin((deg*PI)/180.f);
    glVertex2f(maxL, 0);
    glEnd();

    
}

void displayCallback() {
    //clear frame buffer
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawGrid();
    drawMap();
    drawPlayer();
    castRays();
    // reset model view matrix (rotation scale)

    glutSwapBuffers();
    //flush (finish draw)
    // glFlush();
}

void reshape(int w, int h) {
    //specify viewport
    glViewport(0,0,w,h);

    //projection
    // reset parameter of GL_PROJECTION
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //set projection mode to orthographic 2d
    gluOrtho2D(-10, 10, -10, 10);

    //change matrix mode back
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
}
