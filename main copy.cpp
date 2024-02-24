#include<windows.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>


void displayCallback();
void reshape(int, int);
void timer(int);
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
    init();
    glutMainLoop();
}

float x_position = -10.0;
int state = 1;
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
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

void displayCallback() {
    //clear frame buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // reset model view matrix (rotation scale)
    glLoadIdentity();
    // setup
    glPointSize(10.0);
    // specify mode to draw
    // point or triangles or rectangle (polygon)
    //d raw vertices
    glBegin(GL_POLYGON);

    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(x_position,1.0);
    glVertex2f(x_position,-1.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(x_position+2,-1.0);
    glVertex2f(x_position+2,1.0);

    //end draw vertices
    glEnd();

    glutSwapBuffers();
    //flush (finish draw)
    //glFlush();
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
