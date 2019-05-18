//
//  main.cpp
//  lab3
//
//  Created by Vsevolod Molchanov on 03/04/2019.
//  Copyright © 2019 Vsevolod Molchanov. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <math.h>

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );
void drawCube( GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength);
void drawConicalCylinder(GLdouble a, GLdouble b, int points_am, bool isFill);

int screenWidth, screenHeight;
GLfloat rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
GLfloat translationX = 0.0f, translationY = 0.0f, translationZ = 0.0f;
GLenum m = GL_LINE;
GLfloat k = 1;
GLfloat offset = 0;
GLint n = 18 ;
GLint frags = 1;
bool isFirst = true;
bool isCabinet30 = false, isCabinet45 = false;

const GLfloat m30[16] {
    1, 0, 0, 0,
    0, 1, 0, 0,
    -0.5f * cosf(63.43), -0.5f * sinf(63.43), 1, 0,
    0, 0, 0, 1
};
const GLfloat m45[16] {
    1, 0, 0, 0,
    0, 1, 0, 0,
    -0.5f * cosf(95.06), -0.5f * sinf(95.06), 1, 0,
    0, 0, 0, 1
};

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
    const GLfloat speed = 10;
    
    if ( action == GLFW_PRESS || action == GLFW_REPEAT )
    {
        switch ( key )
        {
            case GLFW_KEY_SPACE:
                (m == GL_LINE)? m = GL_FILL : m = GL_LINE;
                break;
            case GLFW_KEY_UP:
                rotationX -= speed;
                break;
            case GLFW_KEY_DOWN:
                rotationX += speed;
                break;
            case GLFW_KEY_RIGHT:
                rotationY += speed;
                break;
            case GLFW_KEY_LEFT:
                rotationY -= speed;
                break;
            case GLFW_KEY_D:
                rotationZ -= speed;
                break;
            case GLFW_KEY_A:
                rotationZ += speed;
                break;
            case GLFW_KEY_Q:
                translationX -= speed*5;
                break;
            case GLFW_KEY_W:
                translationX += speed*5;
                break;
            case GLFW_KEY_E:
                translationY += speed*5;
                break;
            case GLFW_KEY_R:
                translationY -= speed*5;
                break;
            case GLFW_KEY_T:
                translationZ -= speed*5;
                break;
            case GLFW_KEY_Y:
                translationZ += speed*5;
                break;
            case GLFW_KEY_B:
                (k > 2.9) ? k = 3 : k += 0.1;
                break;
            case GLFW_KEY_S:
                (k < 0.2) ? k = 0.1 : k -= 0.1;
                break;
            case GLFW_KEY_0:
                isCabinet30 = isCabinet45 = false;
                break;
            case GLFW_KEY_3:
                isCabinet30 = true;
                isCabinet45 = false;
                break;
            case GLFW_KEY_4:
                isCabinet45 = true;
                isCabinet30 = false;
                break;
            case GLFW_KEY_EQUAL:
                n++;
                break;
            case GLFW_KEY_MINUS:
                n--;
                break;
            case GLFW_KEY_1:
                frags++;
                break;
            case GLFW_KEY_2:
                (frags < 2)? frags = 1 : frags--;
                break;
        }
        
    }
    
}

void drawConicalCylinder(GLdouble a, GLdouble b, int points_am, bool isFill) {
    double step = (double)1 / (double)points_am;
    double h = 100;
    double d = double(2) * h / frags;
    double c = double(3) / frags;
    
    GLenum rememberM = m;
    if (!isFill) {
        m = GL_LINE;
    }
    glPolygonMode(GL_FRONT_AND_BACK, m);
    m = rememberM;
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, -h);
    for (double angle = -step * M_PI; angle <= 2 * M_PI; angle += step * M_PI) {
        glColor3f((cos(angle) + 1) / 2, (sin(angle) + 1) / 2, 0.7f);
        glVertex3f(4*a*sin(angle), 4*b*cos(angle), -h);
    }
    glEnd();
    
    for (double i = 0; i < frags; i++) {
        glBegin(GL_QUAD_STRIP);
        for (double angle = 0; angle <= 2 * M_PI; angle += step * M_PI) {
            glColor3f((sin(angle) + 1) / 2, (cos(angle) + 1) / 2, 0.7f);
            glVertex3f((1 + i * c)*a*sin(angle), (1 + i * c)*b*cos(angle), h - (i * d));
            glVertex3f((1 + (i + 1) * c)*a*sin(angle), (1 + (i + 1) * c)*b*cos(angle), h - ((i + 1) * d));
        }
        glVertex3f((1 + i * c)*a*sin(0), (1 + i * c)*b*cos(0), h - (i * d));
        glVertex3f((1 + (i + 1) * c)*a*sin(0), (1 + (i + 1) * c)*b*cos(0), h - ((i + 1) * d));
        glEnd();
    }
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, h);
    for (double angle = -step * M_PI; angle <= 2 * M_PI; angle += step * M_PI) {
        glColor3f((cos(angle) + 1) / 2, (sin(angle) + 1) / 2, 0.7f);
        glVertex3f(a*sin(angle), b*cos(angle), h);
    }
    glEnd();
}


void drawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength) {
    GLfloat halfSideLength = edgeLength * 0.5f;
    
    GLfloat vertices[] = {
        // front face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left
        
        // back face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom left
        
        // left face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left
        
        // right face
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left
        
        // top face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom left
        
        // bottom face
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength  // bottom left
    };
    
    GLfloat color[] = {
        
        1,1,1,
        1,1,1,
        1,1,1,
        1,1,1,
        
        1,1,1,
        1,1,1,
        1,1,1,
        1,1,1,
        
        1,1,1,
        1,1,1,
        1,1,1,
        1,1,1,
        
        1,1,1,
        1,1,1,
        1,1,1,
        1,1,1,
        
        1,1,1,
        1,1,1,
        1,1,1,
        1,1,1,
        
        1,1,1,
        1,1,1,
        1,1,1,
        1,1,1
    };
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, color);
    glDrawArrays(GL_QUADS, 0, 24);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}


int main(int argc, const char * argv[]) {
    GLFWwindow *window;
    
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Lab2. Drawing a conical cylinder", NULL, NULL );
    
    glfwSetKeyCallback( window, keyCallback );
    glfwSetInputMode( window, GLFW_STICKY_KEYS, 1 );
    
    
    int screenWidth, screenHeight;
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    glfwMakeContextCurrent( window );
    
    glEnable(GL_DEPTH_TEST);
    
    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ) )
    {
        glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
        glViewport( 0.0f, 0.0f, screenWidth, screenHeight );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, screenWidth, 0, screenHeight, -screenWidth, screenWidth );
        if (isCabinet30) {
            glMultMatrixf(m30);
        }
        if (isCabinet45) {
            glMultMatrixf(m45);
        }
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity( );
        
        GLfloat halfScreenWidth = screenWidth / 2;
        GLfloat halfScreenHeight = screenHeight / 2;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Drawing small cube
        
        GLfloat xSmall = screenWidth / 25, ySmall = screenHeight / 14;
        
        glPushMatrix( );
        glTranslatef( xSmall, ySmall, -500 );
        if (!isCabinet30 && !isCabinet45) { // just rotating small cube without projection so it looks pretty
            glRotatef( -20, 1, 0, 0 );
            glRotatef( -60, 0, 1, 0 );
        }
        glTranslatef( -xSmall, -ySmall, 500 );
        
        drawCube( xSmall, ySmall, -500, xSmall);
        
        glPopMatrix();
        
        // Drawing conical cylinder
        
        glPushMatrix( );
        glTranslatef( halfScreenWidth + translationX, halfScreenHeight + translationY, -500 - translationZ );
        glRotatef( rotationX, 1, 0, 0 );
        glRotatef( rotationY, 0, 1, 0 );
        glRotatef( rotationZ, 0, 0, 1 );
        glScalef(k, k, k);
        
        drawConicalCylinder(50, 50, n, true);

        glTranslatef( -halfScreenWidth, -halfScreenHeight, 500);
        
        
        glPopMatrix();
        
        glfwSwapBuffers( window );
        
        glfwPollEvents( );
    }
    
    glfwTerminate( );
    
    return 0;
}

