
//  main.cpp
//  lab2
//
//  Created by Vsevolod Molchanov on 06/03/2019.
//  Copyright © 2019 Vsevolod Molchanov. All rights reserved.


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <math.h>

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void drawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength, bool isFill);

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat rotationZ = 0.0f;
GLfloat translationX = 0.0f;
GLfloat translationY = 0.0f;
GLfloat translationZ = 0.0f;
GLenum m = GL_LINE;
GLfloat f = 45; // f = 0 for the cube without projection
GLfloat k = 1;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    const GLfloat speed = 10;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
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
                (k >= 1.5) ? k = 1.5 : k += 0.1;
                break;
            case GLFW_KEY_S:
                (k <= 0.2) ? k = 0.1 : k -= 0.1;
                cout << k << endl;
                break;
        }


    }

}

void drawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength, bool isFill) {
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

        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,

        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,

        1,1,0,
        1,1,0,
        1,1,0,
        1,1,0,

        0,1,1,
        0,1,1,
        0,1,1,
        0,1,1,

        1,0,1,
        1,0,1,
        1,0,1,
        1,0,1,

        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0
    };

    GLenum rememberM = m;
    if (!isFill) {
        m = GL_LINE;
    }

    glPolygonMode(GL_FRONT_AND_BACK, m);
    if (isFill) {
        glEnableClientState(GL_COLOR_ARRAY);
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, color);
    
    glDrawArrays(GL_QUADS, 0, 24);
    
    if (isFill) {
        glDisableClientState(GL_COLOR_ARRAY);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    m = rememberM;
}

int main(int argc, const char * argv[]) {
    GLFWwindow *window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab2. Drawing a cube with cabinet projection", NULL, NULL);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLfloat m[] {
        1, 0, 0, 0,
        0, 1, 0, 0,
        -0.5f * cosf(f), -0.5f * sinf(f), -1, 0,
        0, 0, 0, 1
    };

    glViewport(0.0f, 0.0f, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glMultMatrixf(m);
    if (f == 0) {
        glLoadIdentity();
    }
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
    GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawing small cube

        glPushMatrix();
        glTranslatef(50, 50, -500);
        if (f == 0) { // just rotating small cube without projection so it looks pretty
            glRotatef(-20, 1, 0, 0);
            glRotatef(-60, 0, 1, 0);
        }
        glTranslatef(-50, -50, 500);

        drawCube(50, 50, -500, 50, false);

        glPopMatrix();

        // Drawing big cube

        glPushMatrix();
        glTranslatef(halfScreenWidth + translationX, halfScreenHeight + translationY, -500 - translationZ);
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);
        glRotatef(rotationZ, 0, 0, 1);
        glScalef(k, k, k);
        glTranslatef(-halfScreenWidth, -halfScreenHeight, 500);

        drawCube(halfScreenWidth, halfScreenHeight, -500, 200, true);

        glPopMatrix();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
