#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>


#define GLCall(x) GLClearError();\
    x;\
    GLLogCall(#x, __FILE__, __LINE__)


void GLClearError ()
{
    while (glGetError() != GL_NO_ERROR);
}


void GLLogCall (const char* function, const char* file, int line)
{
    while (auto error = glGetError())
    {
        std::cout << "[OpenGL Error] "
                  << "(" << std::hex << error << std::dec << ")"
                  << " " << function
                  << " " << file
                  << ":" << line
                  << std::endl;
    }
}


unsigned int
CompileShader (unsigned int type, const std::string& source)
{
    auto address = glCreateShader(type);
    auto src = source.c_str();
    GLCall(glShaderSource(address, 1, &src, nullptr));
    GLCall(glCompileShader(address));

    int result;
    GLCall(glGetShaderiv(address, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
       int length;
        GLCall(glGetShaderiv(address, GL_INFO_LOG_LENGTH, &length));
       char msg[512];
        GLCall(glGetShaderInfoLog(address, 512, &length, msg));
       std::cout << msg << std::endl;
        GLCall(glDeleteShader(address));
       return 0;
    }

    return address;
}


unsigned int
CreateShaderProgram (const std::string& vertexShaderSource,
                     const std::string& fragmentShaderSource)
{
    auto address = glCreateProgram();
    auto vsAddress = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    auto fsAddress = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLCall(glAttachShader(address, vsAddress));
    GLCall(glAttachShader(address, fsAddress));
    GLCall(glLinkProgram(address));
    GLCall(glValidateProgram(address));

    GLCall(glDeleteShader(vsAddress));
    GLCall(glDeleteShader(fsAddress));

    return address;
}


std::string
LoadShader (const std::string& path)
{
    std::stringstream sshader;

    std::ifstream file(path);
    sshader << file.rdbuf();
    file.close();

    std::string shader;
    shader = sshader.str();

    return shader;
}


int main ()
{
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(640, 480, "Hello world!", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    auto err = glewInit();
    if (GLEW_OK != err) {
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW3 Version: " << glfwGetVersionString() << std::endl;

    float positions[6] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                                 static_cast<void*>(0)));

    unsigned int program;
    auto vs = LoadShader("shader.vs");
    auto fs = LoadShader("shader.fs");
    program = CreateShaderProgram(vs, fs);
    GLCall(glUseProgram(program));

    float o = -0.5f;

    bool moved = false;

    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        o += 0.001f;

        glUniform1f(glGetUniformLocation(program, "offset"), o);
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        glfwSwapBuffers(window);

        if (!moved)
        {
            int x, y;
            glfwGetWindowPos(window, &x, &y);
            glfwSetWindowPos(window, ++x, y);
            glfwSetWindowPos(window, --x, y);
            moved = true;
        }

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
