#include <GL/glew.h>
#include <GL/glut.h>

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <chrono>


using namespace std::literals;
#define MSG_ERROR "[ERROR]: "

static unsigned int CompileShader(unsigned int type, const std::string &source) {
    //cria um shader do tipo vertex shader e retorna o id
    unsigned int id = glCreateShader(type);
    //cuidado com as lifetimes
    const char *src = source.c_str();
    //da se o ponteiro da str com a sauce, e diz se que a string é null terminated
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //alloca na STACK a mensagem a receber
        char *message = (char *) alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

//gives string for the shader code
//return the shader id
static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    //compila o vertex shader
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    //compila o fragment shader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //agora para "linkar o programa, o vertex shader e o fragment shader"
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}


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

    float positions[] = {
            -0.5f, -0.5f,
            0.0f, 0.5f,
            0.5f, -0.5f
    };

    unsigned int buffer;
    //cria um buffer com o id buffer
    glGenBuffers(1, &buffer);
    //faz bind ao buffer(seleciona o buffer na maquina de estado)
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //define o tamanho do buffer e mete os dados
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    //activa a ablidade de desenhar no ecra(?)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader =
            R"(#version 330 core
layout(location = 0) in vec4 position;
void main()
{
    gl_Position = position;
}
)";

    std::string fragmentShader =
            R"(#version 330 core
layout(location = 0) out vec4 color;
void main()
{
    //rgba
    color = vec4(1.0, 0.0, 0.0, 1.0);
}
)";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window)) {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}