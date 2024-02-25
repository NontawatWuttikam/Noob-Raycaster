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
struct Point 
{ 
    float x; 
    float y; 
}; 
  
// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are collinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 

float orientation(Point p, Point q, Point r) 
{ 
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    float val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // collinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 

bool onSegment(Point p, Point q, Point r) { 
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
       return true; 
  
    return false; 
} 
  
// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(Point p1, Point q1, Point p2, Point q2) 
{ 
    // Find the four orientations needed for general and 
    // special cases 
    float o1 = orientation(p1, q1, p2); 
    float o2 = orientation(p1, q1, q2); 
    float o3 = orientation(p2, q2, p1); 
    float o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  

    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
} 

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
float playerX = 1.0f;
float playerY = 4.0f;
double playerAngle = 220.0;
double rays_t[] = {0}; // rays length, for simplicity lets cast 1 ray so the we don't fuck up
const int worldSize = 20;
const int worldMap[worldSize][worldSize] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
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
bool isPress = false;

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

int hitSomething(float oX, float oY, float pX, float pY) {
    //TODO: add corner hit detection
    // check wether current traced point of the ray at pX and pY hit something
    
    // not hit anything
    if (isPress) cout << "rayX,rayY " << pX << " " << pY << "\n";

    //iterate over maps 
    int hit = 0;
    // allow intersec count = 1 (hit the obstacle) more than this is count as went through (violate)
    int intersecPatient = 0;
    float epsilon = 0.0005;
    for (int i = 0; i < worldSize; i++) {
        for (int j = 0; j < worldSize; j++) {
            if (worldMap[j][i] == 1) {
                // construct facets of squared wall from ij
                int facets[4][2][2] = {{{i,j}, {i+1, j}}, {{i,j}, {i, j+1}}, {{i+1,j}, {i+1, j+1}}, {{i,j+1}, {i+1, j+1}}};
                for (int k = 0; k < 4; k++) {
                    // reject ray that went through wall
                    Point wallA = Point();
                    wallA.x = facets[k][0][0] + epsilon;
                    wallA.y = facets[k][0][1] + epsilon;
                    Point wallB = Point();
                    wallB.x = facets[k][1][0] + epsilon;
                    wallB.y = facets[k][1][1] + epsilon;
                    Point rayA = Point();
                    rayA.x = pX ;
                    rayA.y = pY;
                    Point rayB = Point();
                    rayB.x = oX;
                    rayB.y = oY;
                    if(doIntersect(wallA, wallB, rayA, rayB)) {
                        intersecPatient++;
                        if (isPress) cout << "went through wall " << i << " " << j << " side " << k << "\n";
                        if (isPress) cout << "wall " << wallA.x << " " << wallA.y << " " << wallB.x << " " << wallB.y <<"\n";
                        if (isPress) cout << "ray " << rayA.x << " " << rayA.y << " " << rayB.x << " " << rayB.y <<"\n";
                        if (intersecPatient > 1) return 2;
                        continue;
                    }

                    // check hit
                    if (facets[k][0][0] == pX && pX - int(pX) == 0) {
                        bool isHit = facets[k][0][1] <= pY && facets[k][1][1] >= pY;
                        if (isHit) { 
                            if (isPress) cout << "hitOnY " << i << " " << j << "\n";
                            hit = 1;
                        }
                    }
                    if (facets[k][0][1] == pY && pY - int(pY) == 0) {
                        bool isHit = facets[k][0][0] <= pX && facets[k][1][0] >= pX;
                        if (isHit) { 
                            if (isPress) cout << "hitOnX " << i << " " << j << "\n";
                            hit = 1;
                        }
                    }
                }
            }
        }
    }
    return hit;

}

void castRays() {
    
    //DDA Raycaster naive implementation

    // transform player from viewport to world coord
    float pX = playerX - offsetX;
    float pY = worldSize - (playerY - offsetY);
    if (isPress) cout <<"pXpY"<< pX << " " << pY << "\n";
    
    // check quadrant of current player angle
    int quadrant = isAtQuadrant(playerAngle);
    float deg = playerAngle;
    double lx = 0;
    double ly = 0;
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

    int count = 0;
    lx = xlen/cos((deg*PI)/180.f);
    ly = ylen/sin((deg*PI)/180.f);
    if (isPress) cout <<"playerAngle"<< playerAngle << "\n";
    if (isPress) cout <<"deg"<< deg << "\n";
    if (isPress) cout <<"xlen, ylen "<< xlen << " " << ylen << "\n";
    if (isPress) cout <<"cos, sin "<< std::cos((deg*PI)/180.f) << " " << std::sin((deg*PI)/180.f) << "\n";

    
    //check for lx,ly whether the rays hit something b4 incremental
    float uPlayerRotVecX = std::cos((playerAngle*PI)/180.f);
    float uPlayerRotVecY = -1*std::sin((playerAngle*PI)/180.f);
    if (isPress) cout << "uPlayerRotVec " << uPlayerRotVecX << " " << uPlayerRotVecY << "\n";
    if (isPress) cout << "playerPos " << pX << " " << pY << "\n";
    if (isPress) cout << "initial LxLy " << lx << " " << ly << "\n";

    int hitOnLx = hitSomething(pX, pY, pX + (lx*uPlayerRotVecX), pY + (lx*uPlayerRotVecY));
    int hitOnLy = hitSomething(pX, pY, pX + (ly*uPlayerRotVecX), pY + (ly*uPlayerRotVecY));

    if (hitOnLx == 1 || hitOnLy == 1) {
        if (isPress) cout << "hitOnLx or Ly" << "\n\n";
        // return;
    }
    else {
        while (true) {
            // cout << hitOnLx << " " << hitOnLy << "\n";
            if (lx <= ly && hitOnLx != 2) { // not went through wall
                xlen += 1;
                lx = xlen/cos((deg*PI)/180.f);
                hitOnLx = hitSomething(pX, pY, pX + (lx*uPlayerRotVecX), pY + (lx*uPlayerRotVecY));
                if (hitOnLx == 1) break;
            }
            if (ly < lx && hitOnLy != 2) {
                ylen += 1;
                ly = ylen/sin((deg*PI)/180.0);
                hitOnLy = hitSomething(pX, pY, pX + (ly*uPlayerRotVecX), pY + (ly*uPlayerRotVecY));
                if (hitOnLy == 1) break;
            }
        }
    }

    float maxL = max(lx , ly);
    if (hitOnLy && hitOnLx) maxL = min(lx , ly);

    if (isPress) cout <<"xlen', ylen' "<< xlen << " " << ylen << "\n";
    if (isPress) cout <<"LxLy"<< lx << " " << ly << "\n";
    if (isPress) cout <<"maxL"<< maxL << "\n";

    //draw ray
    glLoadIdentity();
    glTranslatef(playerX, playerY, 0.0f);
    glRotatef(playerAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    double uVecX = cos((deg*PI)/180.f);
    double uVecY = sin((deg*PI)/180.f);
    glVertex2f(maxL, 0);
    glEnd();

    if (isPress) cout << "\n";

    
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
