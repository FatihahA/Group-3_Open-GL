#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const int SIDES = 6;
const float RADIUS = 0.4f;
const float HALF_HEIGHT = 0.6f;

float rotY = 0.0f;

// Slightly different shades of the same dark blue for depth
float sideDark[3]   = {0.05f, 0.18f, 0.45f};  // darker side faces
float sideMid[3]    = {0.08f, 0.25f, 0.60f};  // lighter side faces
float topColor[3]   = {0.10f, 0.30f, 0.70f};  // top
float bottomColor[3]= {0.04f, 0.14f, 0.35f};  // bottom

void drawHexPrism() {
    float angleStep = 2.0f * M_PI / SIDES;

    // Side faces — alternate between two close shades for subtle depth
    for (int i = 0; i < SIDES; i++) {
        float a1 = i * angleStep;
        float a2 = (i + 1) * angleStep;

        float x1 = RADIUS * cos(a1), z1 = RADIUS * sin(a1);
        float x2 = RADIUS * cos(a2), z2 = RADIUS * sin(a2);

        if (i % 2 == 0) glColor3fv(sideMid);
        else            glColor3fv(sideDark);

        glBegin(GL_QUADS);
            glVertex3f(x1, -HALF_HEIGHT, z1);
            glVertex3f(x2, -HALF_HEIGHT, z2);
            glVertex3f(x2,  HALF_HEIGHT, z2);
            glVertex3f(x1,  HALF_HEIGHT, z1);
        glEnd();
    }

    // Top face
    glColor3fv(topColor);
    glBegin(GL_POLYGON);
    for (int i = 0; i < SIDES; i++) {
        float a = i * angleStep;
        glVertex3f(RADIUS * cos(a), HALF_HEIGHT, RADIUS * sin(a));
    }
    glEnd();

    // Bottom face
    glColor3fv(bottomColor);
    glBegin(GL_POLYGON);
    for (int i = SIDES - 1; i >= 0; i--) {
        float a = i * angleStep;
        glVertex3f(RADIUS * cos(a), -HALF_HEIGHT, RADIUS * sin(a));
    }
    glEnd();
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL - Rotating Hexagon", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)WIDTH / HEIGHT;
    float fov = 45.0f * M_PI / 180.0f;
    float near = 0.1f, far = 100.0f;
    float f = 1.0f / tan(fov / 2.0f);
    float proj[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far+near)/(near-far), -1,
        0, 0, (2*far*near)/(near-far), 0
    };
    glLoadMatrixf(proj);
    glMatrixMode(GL_MODELVIEW);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -2.5f);
        glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);

        drawHexPrism();

        rotY += 0.5f;
        if (rotY >= 360.0f) rotY -= 360.0f;

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
