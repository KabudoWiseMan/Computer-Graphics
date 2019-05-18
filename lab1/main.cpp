//
//  main.cpp
//  lab1
//
//  Created by Vsevolod Molchanov on 26/02/2019.
//  Copyright © 2019 Vsevolod Molchanov. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

using namespace std;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void draw(GLfloat *vertices, int n, bool isColor);
void prepareCircleVerts(GLfloat *allCircleVertices, GLfloat x, GLfloat y, GLfloat radius, GLint numberOfSides);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

float isTriangle = false, isSquare = false, isCircle = false;
int numberOfSides = 360;
float seconds;

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
//        cout << xpos << " : " << ypos << endl;
        if(xpos >= 90 && xpos <= 180 && ypos >= 10 && ypos <= 100) {
            cout << "Square" << endl;
            isSquare = true;
            isTriangle = false;
            isCircle = false;
        } else if(xpos >= 270 && xpos <= 360 && ypos >= 10 && ypos <= 100) {
            cout << "Triangle" << endl;
            isSquare = false;
            isTriangle = true;
            isCircle = false;
        } else if(xpos >= 450 && xpos <= 540 && ypos >= 10 && ypos <= 100) {
            cout << "Circle" << endl;
            isSquare = false;
            isTriangle = false;
            isCircle = true;
        } else {
            isSquare = false;
            isTriangle = false;
            isCircle = false;
            numberOfSides = 360;
        }
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
//    std::cout << key << std::endl;
    
    // actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
    if (key == GLFW_KEY_SPACE && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        std::cout << "Space Key Pressed" << std::endl;
        if(numberOfSides > 90) {
            numberOfSides-=50;
        }
        numberOfSides--;
        if (numberOfSides < 5) {
            numberOfSides = 360;
        }
    }
}

void prepareCircleVerts(GLfloat *allCircleVertices, GLfloat x, GLfloat y, GLfloat radius, GLint numberOfSides) {
    GLint numberOfVertices = numberOfSides + 2;
    GLfloat twicePi = 2.0f * M_PI;
    GLfloat circleVerticesX[numberOfVertices], circleVerticesY[numberOfVertices], circleVerticesZ[numberOfVertices];
    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesZ[0] = 0;
    for (int i = 1; i < numberOfVertices; i++) {
        circleVerticesX[i] = x + (radius * cos( i *  twicePi / numberOfSides));
        circleVerticesY[i] = y + (radius * sin( i * twicePi / numberOfSides));
        circleVerticesZ[i] = 0;
    }
    
    for (int i = 0; i < numberOfVertices; i++) {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[( i * 3 ) + 1] = circleVerticesY[i];
        allCircleVertices[( i * 3 ) + 2] = circleVerticesZ[i];
    }
}

void draw(GLfloat *vertices, int n, bool isColor) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    if(n == 3) {
        if(isColor) {
            GLfloat color[] = {
                seconds, 0, 0,
                0, seconds, 0,
                0, 0, seconds
            };
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, color);
            glDrawArrays(GL_TRIANGLES, 0, n);
            glDisableClientState(GL_COLOR_ARRAY);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, n);
        }
    }
    if(n == 4) {
        if(isColor) {
            GLfloat color[] = {
                seconds, 0, 0,
                seconds, 0, 0,
                0, 0, 0,
                0, 0, 0
            };
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, color);
            glDrawArrays(GL_QUADS, 0, n);
            glDisableClientState(GL_COLOR_ARRAY);
        } else {
            glDrawArrays(GL_QUADS, 0, n);
        }
    }
    if(n > 4) {
        glDrawArrays(GL_TRIANGLE_FAN, 0, n);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

int main(int argc, const char * argv[]) {
    GLFWwindow *window;
    
    /* Initialize the library */
    if(!glfwInit()) {
        return -1;
    }
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab1. Simple drawing", NULL, NULL);
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    if(!window) {
        glfwTerminate();
        return -2;
    }
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glViewport(0.0f, 0.0f, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /* Buttons */
    GLfloat buttonVerts1[] = {
        180, 470, 0.0, // top right corner
        90, 470, 0.0, // top left corner
        90, 380, 0.0, // bottom left corner
        180, 380, 0.0 // bottom right corner
    };
    GLfloat buttonVerts2[] = {
        315, 470, 0.0, // top right corner
        270, 380, 0.0, // top left corner
        360, 380, 0.0, // bottom left corner
    };
    GLfloat buttonVerts3[362 * 3];
    prepareCircleVerts(buttonVerts3, 495, 425, 45, 360);
    
    /* Figures */
    GLfloat squareVerts[] = {
        470, 360, 0.0, // top right corner
        150, 360, 0.0, // top left corner
        150, 50, 0.0, // bottom left corner
        470, 50, 0.0 // bottom right corner
    };
    GLfloat triangleVerts[] = {
        310, 360, 0.0, // top right corner
        150, 50, 0.0, // top left corner
        470, 50, 0.0, // bottom left corner
    };
    
    glfwSetTime(10);
    
    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* Drawing a square button */
        draw(buttonVerts1, 4, false);
        
        /* Drawing a triangel button */
        draw(buttonVerts2, 3, false);
        
        /* Drawing a circle button */
        draw(buttonVerts3, 362, false);
        
        if(seconds > 1) {
            seconds = 1;
        } else {
            seconds = glfwGetTime()/100;
        }
        std::cout << seconds*100 << std::endl;
        
        /* Render here */
        if (isSquare) {
            draw(squareVerts, 4, true);
        }
        if (isTriangle) {
            draw(triangleVerts, 3, true);
        }
        if (isCircle) {
            GLfloat circleVerts[(numberOfSides + 2) * 3];
            prepareCircleVerts(circleVerts, 310, 205, 160, numberOfSides);
            draw(circleVerts, numberOfSides + 2, false);
        }
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    
    return 0;
}
