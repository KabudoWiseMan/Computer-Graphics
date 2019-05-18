//
//  main.cpp
//  lab5
//
//  Created by Vsevolod Molchanov on 25/04/2019.
//  Copyright © 2019 Vsevolod Molchanov. All rights reserved.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int screenWidth = 800;
int screenHeight = 600;

GLfloat rotationX = 0;
GLfloat rotationY = 0;
GLfloat rotationZ = 0;

GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;

GLfloat polygonL = 0.3f;

bool clippMode = false;
bool normMode = false;

class Point {
public:
    GLfloat x;
    GLfloat y;
    GLfloat z;
    
    Point(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
};

class Line {
public:
    Point p0;
    Point p1;
    
    Point n;
    
    bool disp = true;
    
    Line(const Point &p0, const Point &p1) : p0(p0), p1(p1), n(Point(0, 0, 0)) {}
    
    void display() {
        if (disp) {
            glColor3d(0.8f, 0, 0);
            glVertex3d(p0.x, p0.y, p0.z);
            glVertex3d(p1.x, p1.y, p1.z);
        }
    }
    
};

class Plane {
public:
    Point A;
    Point B;
    Point C;
    Point D;
    
    Point n;
    
    Plane(const Point &A, const Point &B, const Point &C, const Point &D) : A(A), B(B), C(C), D(D), n(Point(0, 0, 0)) {
    }
    
    Point compN() {
        GLfloat Vector[3], Vector2[3];
        
        //3 points of Vector quad
        GLfloat v1[] = {A.x, A.y, A.z};
        GLfloat v2[] = {B.x, B.y, B.z};
        GLfloat v3[] = {C.x, C.y, C.z};
        
        
        Vector[0] = v3[0] - v1[0];
        Vector[1] = v3[1] - v1[1];
        Vector[2] = v3[2] - v1[2];
        
        Vector2[0] = v2[0] - v1[0];
        Vector2[1] = v2[1] - v1[1];
        Vector2[2] = v2[2] - v1[2];
        
        GLfloat Product[3];
        
        //Cross product formula
        Product[0] = (Vector[1] * Vector2[2]) - (Vector[2] * Vector2[1]);
        Product[1] = (Vector[2] * Vector2[0]) - (Vector[0] * Vector2[2]);
        Product[2] = (Vector[0] * Vector2[1]) - (Vector[1] * Vector2[0]);
        
        Point an(0, 0, 0);
        
        an.x = Product[0];
        an.y = Product[1];
        an.z = Product[2];
        
        return an;
    }
    
    void display(bool norm) {
        glColor3d(1, 1, 1);
        glVertex3d(A.x, A.y, A.z);
        glVertex3d(B.x, B.y, B.z);
        
        glVertex3d(B.x, B.y, B.z);
        glVertex3d(C.x, C.y, C.z);
        
        glVertex3d(C.x, C.y, C.z);
        glVertex3d(D.x, D.y, D.z);
        
        glVertex3d(D.x, D.y, D.z);
        glVertex3d(A.x, A.y, A.z);
        
        if (norm) {
            GLfloat dx = (A.x + B.x + C.x + D.x) / 4;
            GLfloat dy = (A.y + B.y + C.y + D.y) / 4;
            GLfloat dz = (A.z + B.z + C.z + D.z) / 4;
            glColor3f(1, 0, 1);
            glVertex3d(dx, dy, dz);
            glVertex3d(n.x + dx, n.y + dy, n.z + dz);
        }
    }
};

vector<Plane> polygon;
vector<Line> lines;

void AddVertex(){
    cout << "Input line: ";
    GLfloat x0;
    GLfloat y0;
    GLfloat z0;
    
    GLfloat x1;
    GLfloat y1;
    GLfloat z1;
    
    scanf("%f",&x0);
    scanf("%f",&y0);
    scanf("%f",&z0);
    
    scanf("%f",&x1);
    scanf("%f",&y1);
    scanf("%f",&z1);
    
    lines.push_back(Line(Point(x0,y0,z0), Point(x1,y1,z1)));
}


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_F:
                clippMode = !clippMode;
                break;
            case GLFW_KEY_N:
                normMode = !normMode;
                break;
            case GLFW_KEY_UP:
                rotationX += 5;
                break;
            case GLFW_KEY_DOWN:
                rotationX -= 5;
                break;
            case GLFW_KEY_RIGHT:
                rotationY -= 5;
                break;
            case GLFW_KEY_LEFT:
                rotationY += 5;
                break;
            case GLFW_KEY_A:
                rotationZ += 5;
                break;
            case GLFW_KEY_D:
                rotationZ -= 5;
                break;
            case GLFW_KEY_I:
                AddVertex();
                break;
        }
    }
}

void onResize(GLFWwindow *window, int width, int height) {
    if(width>height) {
        glViewport((width-height)/2, 0, min(width,height), min(width,height));
    } else {
        glViewport(0, (height-width)/2, min(width,height), min(width,height));
    }
}

double dot(Point p1, Point p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

int chopCI(double &tIn, double &tOut, double num, double denom) {
    double tHit;
    if (denom < 0) {
        tHit = num / denom;
        if (tHit > tOut) return 0;
        else if (tHit > tIn) tIn = tHit;
        
    } else if (denom > 0) {
        tHit = num / denom;
        if (tHit < tOut) tOut = tHit;
    } else {
        if (num <= 0) return 0;
        return 1;
    }
    return 1;
}


int main(int argc, const char * argv[]) {
    GLFWwindow *window;
    
    if (!glfwInit()) {
        return -1;
    }
    
    window = glfwCreateWindow(screenWidth, screenHeight, "Lab 5", NULL, NULL);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, onResize);
    
//    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    glfwMakeContextCurrent(window);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    polygon.push_back(Plane(Point(0.0f, +0.3f, +0.3f),
                            Point(+0.248101f, +0.3f, +0.146706f),
                            Point(+0.248101f, -0.3f, +0.146706f),
                            Point(0.0f, -0.3f, +0.3f)));
    polygon[0].n = Point(0.0f, 0, 0.5f);
    
    polygon.push_back(Plane(Point(+0.248101f, +0.3f, +0.146706f),
                            Point(+0.2424615f, +0.3f, -0.1677525f),
                            Point(+0.2424615f, -0.3f, -0.1677525f),
                            Point(+0.248101f, -0.3f, +0.146706f)));
    polygon[1].n = Point(0.0f, 0, -0.5f);

    polygon.push_back(Plane(Point(-0.3f, +0.3f, +0.3f),
                            Point(-0.3f, +0.3f, -0.3f),
                            Point(-0.3f, -0.3f, -0.3f),
                            Point(-0.3f, -0.3f, +0.3f)));
    polygon[2].n = Point(-0.5f, 0, 0);

    polygon.push_back(Plane(Point(+0.3f, +0.3f, +0.3f),
                            Point(+0.3f, +0.3f, -0.3f),
                            Point(+0.3f, -0.3f, -0.3f),
                            Point(+0.3f, -0.3f, +0.3f)));
    polygon[3].n = Point(0.5f, 0, 0);

    polygon.push_back(Plane(Point(-0.3f, +0.3f, +0.3f),
                            Point(-0.3f, +0.3f, -0.3f),
                            Point(+0.3f, +0.3f, -0.3f),
                            Point(+0.3f, +0.3f, +0.3f)));
    polygon[4].n = Point(0, 0.5f, 0);

    polygon.push_back(Plane(Point(-0.3f, -0.3f, +0.3f),
                            Point(-0.3f, -0.3f, -0.3f),
                            Point(+0.3f, -0.3f, -0.3f),
                            Point(+0.3f, -0.3f, +0.3f)));
    polygon[5].n = Point(0, -0.5f, 0);
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
//        glRotated(-30.0f, 1, 0, 0);
//        glRotated(-40.0f, 0, 1, 0);
        
        glRotatef( rotationX, 1, 0, 0 );
        glRotatef( rotationY, 0, 1, 0 );
        glRotatef( rotationZ, 0, 0, 1 );
        
        glBegin(GL_LINES);
        
        for (int i = 0; i < polygon.size(); i++) {
            polygon[i].display(normMode);
        }
        if (!clippMode) {
            for (int i = 0; i < lines.size(); i++) {
                lines[i].display();
            }
        }
        
        if (clippMode) {
            for (int i = 0; i < lines.size(); i++) {
                double num, denom;
                double tIn = 0.0, tOut = 1.0;
                Point c = Point(lines[i].p1.x - lines[i].p0.x,
                                lines[i].p1.y - lines[i].p0.y,
                                lines[i].p1.z - lines[i].p0.z);
                
                for (int j = 0; j < polygon.size(); j++) {
                    Point tmp = Point(polygon[j].A.x - lines[i].p0.x,
                                      polygon[j].A.y - lines[i].p0.y,
                                      polygon[j].A.z - lines[i].p0.z);
                    
                    num = dot(polygon[j].n, tmp);
                    denom = dot(polygon[j].n, c);
                    if (!chopCI(tIn, tOut, num, denom)) {
                        lines[i].disp = false;
                        break;
                    }
                }
                if (tOut < 1.0) {
                    lines[i].p1.x = lines[i].p0.x + c.x * tOut;
                    lines[i].p1.y = lines[i].p0.y + c.y * tOut;
                    lines[i].p1.z = lines[i].p0.z + c.z * tOut;
                }
                
                if (tIn > 0.0) {
                    lines[i].p0.x = lines[i].p0.x + c.x * tIn;
                    lines[i].p0.y = lines[i].p0.y + c.y * tIn;
                    lines[i].p0.z = lines[i].p0.z + c.z * tIn;
                }
                
                for (int i = 0; i < lines.size(); i++) {
                    lines[i].display();
                }
                
            }
        }
        
        glEnd();
        glPopMatrix();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}
