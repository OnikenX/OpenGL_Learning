#include <GL/glew.h>
#include <GL/glut.h>

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std::literals;

/// DEBUGGING
#define MSG_ERROR "[ERROR]: "

#ifdef __MSVC_VER
#include <windows.h>
#define BREAK_POINT DebugBreak()
#else

#include <csignal>

#define BREAK_POINT raise(SIGTRAP)
#endif
#define GLLOG_AND_ASSERT() ASSERT_NO_LOG(GLLogCall( __FILE__, __LINE__, nullptr))

//para melhor debugging usa se o glcall
#define GLCall(x)  x;\
ASSERT_NO_LOG(GLLogCall( __FILE__, __LINE__, #x));


static bool GLLogCall(const char *file, unsigned int line, const char *function) {
    bool no_errors = true;
    while (GLenum error = glGetError()) {
        fprintf(stderr, "[OpenGL Error] (%s:%d) in %s:%d %s\n", glewGetErrorString(error), error, file, line, function);
        no_errors = false;
    }
    return no_errors;
}

/// GLOBAL VARIABLES
//o tempo que demora para haver uma variação das cores
const auto time_interval = 30ms;
//define se a variação se pode ter nas cores
const float variation = 0.02f;




#define ASSERT_NO_LOG(x) if(!(x)) BREAK_POINT
#define ASSERT(x) if(!(x)) assert_error(__FILE__, __LINE__, #x); BREAK_POINT

static void assert_error(const char *file, int line, const char *expression) {
    fprintf(stderr, "Assert error at %s:%d with expression [%s]\n", file, line, expression);
}


struct ShaderProgramSource {
    std::string vertex_source;
    std::string fragment_source;
};
using namespace std::literals;

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}


static ShaderProgramSource ParseShader(const std::string &file) {
    std::ifstream stream(file);

    if (!stream.is_open()) {
        const char *message = "Couldn't open shader file!";
        std::cerr << message << std::endl;
        throw std::exception();
    }
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[(int) type] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source) {
    //cria um shader do tipo vertex shader e retorna o id
    unsigned int id = GLCall(glCreateShader(type));

    //cuidado com as lifetimes
    const char *src = source.c_str();
    //da se o ponteiro da str com a sauce, e diz se que a string é null terminated
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        //alloca na STACK a mensagem a receber
        char *message = (char *) alloca(length * sizeof(char));

        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cerr << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
        std::cerr << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

//gives string for the shader code
//return the shader id
static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = GLCall(glCreateProgram());
    //compila o vertex shader
    unsigned int vs = GLCall(CompileShader(GL_VERTEX_SHADER, vertexShader));
    //compila o fragment shader
    unsigned int fs = GLCall(CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    //agora para "linkar o programa, o vertex shader e o fragment shader"
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}


int main(int argc, char *argv[]) {
    GLFWwindow *window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Hello World", nullptr, nullptr);
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
    std::cout << glGetString(GL_VERSION) << std::endl;

    GLCall(glEnable(GL_DEBUG_OUTPUT));
    float positions[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f
    };

    unsigned int indexes[] = {
            0, 1, 2,
            0, 2, 3
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1,&vao));
    GLCall(glBindVertexArray(vao));

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));//cria um buffer com o id buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));//faz bind ao buffer(seleciona o buffer na maquina de estado)
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions,
                        GL_STATIC_DRAW));//define o tamanho do buffer e mete os dados

    GLCall(glEnableVertexAttribArray(0));    //activa a ablidade de desenhar no ecra(?)
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));//cria um buffer com o id ibo
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));//faz bind ao ibo(seleciona o ibo na maquina de estado)
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes,
                        GL_STATIC_DRAW));//define o tamanho do buffer e mete os dados

    ShaderProgramSource source = ParseShader("res/shaders/basic.shader");

    unsigned int shader = CreateShader(source.vertex_source, source.fragment_source);
    GLCall(glUseProgram(shader));
    unsigned int u_color_location = GLCall(glGetUniformLocation(shader, "u_Color"));
    float colors[]{1.0f, 0.0f, 0.0f};
    GLCall(glUniform4f(u_color_location, colors[0], colors[1], colors[2], 1.0f));
    auto getNow = []() { return (std::chrono::high_resolution_clock::now()); };
    auto start = getNow();
    while (!glfwWindowShouldClose(window)) {

        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)));
        auto now = getNow();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > (time_interval).count()) {
            const uint8_t max = sizeof(colors) / sizeof(float);
            uint8_t ant = max - 1;
            for (uint8_t i = 0; i < max; ++i) {
                if (i != 0)
                    ant = i - 1;
                if (colors[i] > 0.0f && colors[ant] <= 0.0f) {
                    colors[i] -= variation;
                    if (i == max - 1)
                        colors[0] += variation;
                    else
                        colors[i+1] += variation;
                    break;
                }
            }
            GLCall(glUniform4f(u_color_location, colors[0], colors[1], colors[2], 1.0f));
            start = now;
        }
//        GLCall(glDrawArrays(GL_LINES,0, sizeof(positions)/4);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    GLCall(glDeleteProgram(shader));
    glfwTerminate();
    return 0;
}