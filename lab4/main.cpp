//
//  main.cpp
//  lab4
//
//  Created by Vsevolod Molchanov on 17/04/2019.
//  Copyright © 2019 Vsevolod Molchanov. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>

using namespace std;

int startScreenWidth = 1280;
bool isScreenChanged = false;
int screenWidth = 1280, screenHeight = 720;

struct Point {
    int x, y;
};

struct Pixel {
    float r, g, b, alpha;
};

bool isEqual(Pixel p1, Pixel p2) {
    return p1.r == p2.r && p1.g == p2.g && p1.b == p2.b && p1.alpha == p2.alpha;
}

Pixel *buffer = NULL;
vector<Point> curve;
vector<vector<Point>> lines;
int k = 0;

void clearBuffer() {
    Pixel clearColor{0, 0, 0, 1};
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        buffer[i] = clearColor;
    }
}

Pixel contourColor{1, 1, 1, 0.99};
Pixel vertexColor{0, 0, 1, 0.99};
Pixel fillColor{1, 1, 1, 1};
Pixel unfillColor{0, 0, 0, 1};
Pixel lineColor{1, 1, 1, 1};
bool isInputMode = true, isFillMode = false, isPostfiltrationMode = false;
Point startPoint{-1921, -1081};

Pixel *getPixel(Point point) {
    return &buffer[point.y*screenWidth + point.x];
}

void setPixel(Point point, Pixel pixel) {
    Pixel *p = getPixel(point);
    *p = pixel;
}

void makeEdge(Point p1, Point p2) {
    vector<Point> line;
    if (p1.y <= p2.y) {
        for (int y = p1.y; y < p2.y; y++) {
            int x = p1.x + double((y - p1.y) * (p2.x - p1.x)) / double(p2.y - p1.y);
            line.push_back(Point{x, y});
        }
    } else {
        for (int y = p2.y; y < p1.y; y++) {
            int x = p1.x + double((y - p1.y) * (p2.x - p1.x)) / double(p2.y - p1.y);
            line.push_back(Point{x, y});
        }
    }
    lines.push_back(line);
    
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    if (dy >= dx) {
        if (p1.y <= p2.y) {
            for (int y = p1.y; y < p2.y; y++) {
                int x = p1.x + double((y - p1.y) * (p2.x - p1.x)) / double(p2.y - p1.y);
                setPixel(Point{x, y}, contourColor);
            }
        } else {
            for (int y = p2.y; y < p1.y; y++) {
                int x = p1.x + double((y - p1.y) * (p2.x - p1.x)) / double(p2.y - p1.y);
                setPixel(Point{x, y}, contourColor);
            }
        }
    } else {
        if (p1.x <= p2.x) {
            for (int x = p1.x; x < p2.x; x++) {
                int y = p1.y + double((x - p1.x) * (p2.y - p1.y)) / double(p2.x - p1.x);
                setPixel(Point{x, y}, contourColor);
            }
        } else {
            for (int x = p2.x; x < p1.x; x++) {
                int y = p1.y + double((x - p1.x) * (p2.y - p1.y)) / double(p2.x - p1.x);
                setPixel(Point{x, y}, contourColor);
            }
        }
    }
}

void makeContour() {
    for (int i = 0; i < curve.size() - 1; i++) {
        makeEdge(curve[i], curve[i + 1]);
    }
}

void fillContour() {
    for (int i = 0; i < lines.size() - k; i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            for (int sx = lines[i][j].x; sx <= startPoint.x; sx++) {
                Point p{sx, lines[i][j].y};
                if (isEqual(*getPixel(p), fillColor)) {
                    setPixel(p, unfillColor);
                } else {
                    setPixel(p, fillColor);
                }
                
            }
            for (int sx = lines[i][j].x; sx > startPoint.x; sx--) {
                Point p{sx, lines[i][j].y};
                if (isEqual(*getPixel(p), fillColor)) {
                    setPixel(p, unfillColor);
                } else {
                    setPixel(p, fillColor);
                }
                
            }
        }
    }
}

void onResize(GLFWwindow *window, int width, int height) {
    float widthCoef = float(width) / float(screenWidth), heightCoef = float(height) / float(screenHeight);
    for (int i = 0; i < curve.size(); i++) {
        curve[i] = Point{int(round(float(curve[i].x) * widthCoef)), int(round(float(curve[i].y) * heightCoef))};
    }
    if (startPoint.x != -1921 && startPoint.y != -1081) {
        startPoint = Point{int(round(float(startPoint.x) * widthCoef)), int(round(float(startPoint.y) * heightCoef))};
    }
    
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
    
    /* redrawing picture */
    delete [] buffer;
    buffer = new Pixel[screenWidth * screenHeight];
    if (isFillMode) {
        clearBuffer();
        lines.clear();
        makeContour();
        if (startPoint.x != -1921 && startPoint.y != -1081) {
            fillContour();
        }
    }
}

void closingContour() {
    curve.push_back(curve[0]);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if ( action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ENTER:
                if (isInputMode && curve.size() > 2) {
                    isInputMode = false;
                    closingContour();
                }
                break;
            case GLFW_KEY_BACKSPACE:
                clearBuffer();
                curve.clear();
                startPoint.x = -1921;
                startPoint.y = -1081;
                isInputMode = true;
                isFillMode = false;
                isPostfiltrationMode = false;
                lines.clear();
                break;
            case GLFW_KEY_SPACE:
                if (!isFillMode && !isInputMode) {
                    isFillMode = true;
                    clearBuffer();
                    makeContour();
                }
                break;
            case GLFW_KEY_F:
                isPostfiltrationMode = !isPostfiltrationMode;
                break;
            case GLFW_KEY_L:
                if (startPoint.x != -1921 && startPoint.y != -1081) {
                    (k >= (lines.size() - 1))? k = int(lines.size()) - 1 : k++;
                    clearBuffer();
                    lines.clear();
                    makeContour();
                    fillContour();
                }
                break;
            case GLFW_KEY_M:
                if (startPoint.x != -1921 && startPoint.y != -1081) {
                    (k <= 0)? k = 0 : k--;
                    clearBuffer();
                    lines.clear();
                    makeContour();
                    fillContour();
                }
                break;
            case GLFW_KEY_U:
                clearBuffer();
                lines.clear();
                makeContour();
                startPoint.x = -1921;
                startPoint.y = -1081;
                break;
        }
    }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (isInputMode) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if (isScreenChanged) {
                curve.push_back(Point{int(round(2*xpos)), int(round(float(screenHeight) - 2*ypos))});
            } else {
                curve.push_back(Point{int(round(xpos)), int(round(float(screenHeight) - ypos))});
            }
        }
    }
    /* reading the partition line */
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (isFillMode) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
             if (isScreenChanged) {
                 startPoint = Point{int(2*xpos), screenHeight - int(2*ypos)};
             } else {
                 startPoint = Point{int(xpos), screenHeight - int(ypos)};
             }
            fillContour();
        }
    }
}

void drawCurve() {
    if (curve.size() == 1) {
        glBegin(GL_POINTS);
        glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.alpha);
        Point point = curve[0];
        glVertex2f(float(point.x), float(point.y));
        glEnd();
    } else if (curve.size() > 1) {
        glBegin(GL_LINE_STRIP);
        glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.alpha);
        for (int i = 0; i < curve.size(); i++) {
            Point point = curve[i];
            glVertex2f(float(point.x), float(point.y));
        }
        glEnd();
    }
}

void makePostfiltration() {
    glDrawBuffer(GL_BACK);
    glClear(GL_ACCUM_BUFFER_BIT);
    int rasterMask[9][2]{{0, 2}, {1, 2}, {2, 2},
        {0, 1}, {1, 1}, {2, 1},
        {0, 0}, {1, 0}, {2, 0}};
    float intensityMask[9]{1, 2, 1,
        2, 4, 2,
        1, 2, 1};
    
    for (int i = 0; i < 9; i++) {
        glRasterPos2i(rasterMask[i][0], rasterMask[i][1]);
        glDrawPixels(screenWidth, screenHeight, GL_RGBA, GL_FLOAT, &buffer[0]);
        glAccum(GL_ACCUM, intensityMask[i] / 8);
    }
    
    glAccum(GL_RETURN, 1.0);
}

int main(int argc, const char * argv[]) {
    GLFWwindow *window;
    
    if (!glfwInit()) {
        return -1;
    }
    
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    /* make buffer */
    glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, 4);
    glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, 4);
    glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, 4);
    glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, 4);
    
    window = glfwCreateWindow(screenWidth, screenHeight, "Lab4", NULL, NULL);
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    if (screenWidth != startScreenWidth) {
        isScreenChanged = true;
    }
    
    
    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glEnable(GL_ALPHA_TEST);
    
    buffer = new Pixel[screenWidth * screenHeight];
    clearBuffer();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glRasterPos2i(0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        if (!isFillMode) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, screenWidth, 0, screenHeight, -screenWidth, screenWidth);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            drawCurve();
        } else {
            glDrawBuffer(GL_BACK);
            glDrawPixels(screenWidth, screenHeight, GL_RGBA, GL_FLOAT, &buffer[0]);
            if (isPostfiltrationMode) {
                makePostfiltration();
            }
        }
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}
