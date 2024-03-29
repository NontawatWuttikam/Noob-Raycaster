#include<windows.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include <iostream>
#include <math.h>

bool isPress = false;
const double PI = 3.14159265359;
float EPS = 0.00005;
float F_INF = 1e+18;

using namespace std;
void displayCallback();
void reshape(int, int);
void timer(int);
void drawGrid();
void keyboard(unsigned char, int, int);

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
}


template<typename T, typename... Args>
void print(T firstArg, Args... args) {
    if (isPress) {
        std::cout << firstArg; // Print the first argument

        // Print the rest of the arguments space-separated
        ((std::cout << ' ' << args), ...);
        
        std::cout << std::endl; // End the line
    }
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
const int worldSize = 20;
const int worldMap[worldSize][worldSize] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                          {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

void castRays();

void keyboard(unsigned char key, int x, int y) {
    isPress = true;
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

void drawPoint(float x, float y, float r, float g, float b) {
    glLoadIdentity();
    glPointSize(10.0);
    glBegin(GL_POINTS);
    glColor3f(r,g,b);
    glVertex2f(x,y);
    glColor3f(1.0, 1.0, 1.0);
    glEnd();
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
    // glBegin(GL_POLYGON);
    // glColor3f(1.0, 0.0, 0.0);
    // glVertex2f(0.5, 0);
    // glVertex2f(-0.5, 0.3);
    // glVertex2f(-0.5, -0.3);
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

float* findEndOfRay(float ax, float ay, float t, float angle) {
    //check for lx,ly whether the rays hit something b4 incremental
    float uRotVecX = std::cos((angle*PI)/180.f);
    float uRotVecY = -1*std::sin((angle*PI)/180.f);
    float* out = (float*) malloc(2*sizeof(float));
    out[0] = ax + (t*uRotVecX);
    out[1] = ay + (t*uRotVecY);
    return out;
    // print("endOfRay", );
}

bool hitSomething(float pX, float pY) {
    //TODO: add corner hit detection
    // check wether current traced point of the ray at pX and pY hit something
    
    // not hit anything

    //iterate over maps 
    for (int i = 0; i < worldSize; i++) {
        for (int j = 0; j < worldSize; j++) {
            if (worldMap[j][i] == 1) {
                // construct facets of squared wall from ij
                int facets[4][2][2] = {{{i,j}, {i+1, j}}, {{i,j}, {i, j+1}}, {{i+1,j}, {i+1, j+1}}, {{i,j+1}, {i+1, j+1}}};
                for (int k = 0; k < 4; k++) {

                    // DEBUG if, TODO: remove    
                    if (j == 7 && i == 0 && pX <= 8 && pX >= 7) {
                        // print("isHit71", "facet", k, "at", pY,pX, "from", facets[k][0][0], facets[k][0][1] , "to", facets[k][1][0], facets[k][1][1]);
                        // print("facets[k][0][0] - pY < EPS" "is", fabsf(facets[k][0][0] - pY) < EPS);
                        // print("facets[k][0][1] - pX < EPS","is", fabsf(facets[k][0][1] - pX) < EPS);
                        // print("minus result",facets[k][0][0] - pY);
                        // print("minus result",facets[k][0][1] - pX);
                        // print("pY - int(pY) < EPS",pY, "-", int(pY), "< EPS is " , fabsf(roundf(pY) - pY) < EPS);
                        // print("pX - int(pX) < EPS",pX, "-", int(pX), "< EPS is " , fabsf(roundf(pX) - pX) < EPS);
                    }

                    if (fabsf(facets[k][0][0] - pX) < EPS && fabsf(roundf(pX) - pX) < EPS) {
                        bool isHit = facets[k][0][1] <= pY && facets[k][1][1] >= pY;
                        if (isHit) { 
                            return true; 
                        }
                    }
                    else if (fabsf(facets[k][0][1] - pY) < EPS && fabsf(roundf(pY) - pY) < EPS) {
                        bool isHit = facets[k][0][0] <= pX && facets[k][1][0] >= pX;
                        if (isHit) { 
                            return true; 
                        }
                    }
                }
            }
        }
    }
    return false;
}

void castRays() {
    
    //DDA Raycaster naive, verbose, highly unoptimized, implementation

    // transform player from viewport to world coord
    float pX = playerX - offsetX;
    float pY = worldSize - (playerY - offsetY);
    print("pX,pY",pX,pY);
    
    // check quadrant of current player angle
    int quadrant = isAtQuadrant(playerAngle);
    float deg = playerAngle;
    double lx = 0; // ray length (hypotenuse) wrt lx 
    double ly = 0; // ray length (hypotenuse) wrt ly
    float xlen = 0; // length in x axis to step +1
    float ylen = 0; // length in y axis to step + 1

    float ax = floor(pX); // grid origin that player is currently in.
    float ay = floor(pY);
    float vx = 1.0f; // direction to step lx 
    float vy = 1.0f; // direction to step ly
    // cout << "quadrant " << quadrant << "\n";
    switch(quadrant) {
        case 0: xlen = 1.0f - (pX - ax);
                ylen = (pY - ay); 
                break;
        case 1: deg = 180 - playerAngle;
                xlen = (pX - ax);
                ylen = (pY - ay); 
                vx = -1.0f;
                break;
        case 2: deg = playerAngle - 180;
                xlen = (pX - ax);
                ylen = 1.0f - (pY - ay); 
                vx = -1.0f;
                vy = -1.0f;
                break;
        case 3: deg = 360 - playerAngle;
                xlen = 1.0f - (pX - ax);
                ylen = 1.0f - (pY - ay); 
                vy = -1.0f;
                break;
    }
    if (fabs(deg - 90) < EPS) lx = F_INF; // prevent numerical issues when divide by cos90 = 0 must be inf.
    else lx = xlen/cos((deg*PI)/180.f);
    ly = ylen/sin((deg*PI)/180.f);

    print("AxAy",ax,ay);
    print("playerAngle",playerAngle);
    print("deg",deg);
    print("xlen,ylen",xlen,ylen);
    print("lx,ly",lx,ly);
    print("cos,sin ",std::cos((deg*PI)/180.f),std::sin((deg*PI)/180.f));

    float* endOfRaysLx = findEndOfRay(pX, pY, lx, playerAngle); // find initial end of ray given length lx 
    float* endOfRaysLy = findEndOfRay(pX, pY, ly, playerAngle); // find initial end of ray given length ly
    float finalLx = F_INF; // set final ray length to inf
    float finalLy = F_INF; // set final ray length to inf
    bool isLxHit = false; // flag to check if rayLx is hit something
    bool isLyHit = false; // flag to check if rayLy is hit something
    int maxStep = worldSize*2; //maximum step to prvent forever ray cast (edge case, bugs)
    int count = 0;

    while (true) { // tip of the ray is in wall
        if (hitSomething(endOfRaysLx[0], endOfRaysLx[1]) && !isLxHit) {finalLx = lx; isLxHit = true; print(">Red Hit at", endOfRaysLx[0], endOfRaysLx[1]);} // check if RayLx hit any facet of the wall
        if (hitSomething(endOfRaysLy[0], endOfRaysLy[1]) && !isLyHit) {finalLy = ly; isLyHit = true; print(">Green Hit at", endOfRaysLy[0], endOfRaysLy[1]);} // check if RayLy hit any facet of the wall
        if (isLxHit && isLyHit) break; // if both rays hit, stop

        // if not, increment short ray along x,y axis by 1
        if (lx <= ly) {
            xlen += 1.0f;
            lx = xlen/cos((deg*PI)/180.f); // compute the ray len again
        }
        else if (ly < lx) {
            ylen += 1.0f;
            ly = ylen/sin((deg*PI)/180.0); // compute the ray len again
        }

        endOfRaysLx = findEndOfRay(pX, pY, lx, playerAngle); // find the end of ray given origin and ray length Lx
        endOfRaysLy = findEndOfRay(pX, pY, ly, playerAngle); // find the end of ray given origin and ray length Ly
        count++;

        print("Red",count,endOfRaysLx[0],endOfRaysLx[1]);
        print("Green",count,endOfRaysLy[0],endOfRaysLy[1]);
        
        // draw a point on a grid line in both x and y axis which a ray at any step stop at.
        drawPoint(endOfRaysLx[0] + offsetX, -1*(endOfRaysLx[1] + offsetY), 1.0, 0.0, 0.0);
        drawPoint(endOfRaysLy[0] + offsetX, -1*(endOfRaysLy[1] + offsetY), 0.0, 1.0, 0.0);

        // check if the endOfRayLx is outside the map, we won't step further
        if (endOfRaysLx[0] < 0 || endOfRaysLx[0] > worldSize || endOfRaysLx[1] < 0 || endOfRaysLx[1] > worldSize) { // both endofrayLx and endofrayLy are out of bound
            print("Red invalid");
            // break;
            isLxHit = true; // we won't step further
        }
        // check if the endOfRayLx is outside the map, we won't step further
        if (endOfRaysLy[0] < 0 || endOfRaysLy[0] > worldSize || endOfRaysLy[1] < 0 || endOfRaysLy[1] > worldSize) { // both endofrayLx and endofrayLy are out of bound
            print("Green invalid");
            // break;
            isLyHit = true; // we won't step further
        }

        // if weird case happened (I am not John carmack bro :/) stop at maxStep. it is better than stucking in infinite loop.
        count++;
        if (count == maxStep) break;
    }

    float finalL = min(finalLx, finalLy);

    print("FinalendOfRaysLx", endOfRaysLx[0], endOfRaysLx[1]);
    print("FinalendOfRaysLy", endOfRaysLy[0], endOfRaysLy[1]);

    print("xlen,ylen",xlen,ylen);
    print("LxLy",lx,ly);
    print("finalLxLy",finalLx, finalLy);
    print("finalL",finalL);


    //draw ray
    glLoadIdentity();
    glTranslatef(playerX, playerY, 0.0f);
    glRotatef(playerAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0,0);
    double uVecX = cos((deg*PI)/180.f);
    double uVecY = sin((deg*PI)/180.f);
    glVertex2f(finalL, 0);
    glColor3f(1.0, 1.0, 1.0);
    glEnd();

    print("\n");

    
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
    isPress = false;
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
