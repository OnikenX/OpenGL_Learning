#include <GL/glew.h>
#include <GL/glut.h>

#include <GLFW/glfw3.h>


#include <iostream>
#include <chrono>

using namespace std::literals;
#define MSG_ERROR "[ERROR]: "

int main(int argc, char *argv[]) {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << MSG_ERROR << " Can't create an window." << std::endl;
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << MSG_ERROR << "Libraries not loaded correctly." << std::endl;
    }

    /* Loop until the user closes the window */
    bool increment = true;
    float variante = 0.5f;
    auto get_time = []() { return std::chrono::high_resolution_clock::now(); };
    auto start = get_time();

    while (!glfwWindowShouldClose(window)) {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glDrawElements(GL_TRIANGLES, 3, )
        if ((get_time() - start) > 10ms) {
            if (increment) {
                if (variante < 0.75f) {
                    variante += 0.01f;
                } else {
                    increment = false;
                    variante -= 0.01f;
                }
            } else {
                if (variante > 0.25f) {
                    variante -= 0.01f;
                } else {
                    increment = true;
                    variante+=0.01f;
                }
            }
            start = get_time();
        }
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.75f, -0.75f);
        glVertex2f(0.0f, variante);
        glVertex2f(0.75f, -0.75f);
        glColor3b(0, 127, 0);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(-.90f, .90f);
        glVertex2f(.90f, .90f);
        glColor3b(0, 0, 127);

        glVertex2f(-.90f, -.90f);
        glVertex2f(.90f, -.90f);
        glColor3b(127, 0, 0);

        glEnd();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}