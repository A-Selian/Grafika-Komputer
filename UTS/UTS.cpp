#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "utility.h"

using namespace std;

float velocitySecondPointer = -6.0f / 2;
float velocityMinutePointer = -0.1f / 2;
float velocityHourPointer = -0.0083333f / 2;

float currentSecondPointer = 270.0f;
float currentMinutePointer = 270.0f;
float currentHourPointer = 270.0f;

float currentTime = 0.0f;
float lastTime = 0.0f;
float deltaTime = 0.0f;

float inputHour, inputMinute, inputSecond;

float speed = 1.0f;

unsigned int program;

GLint color1Loc, color2Loc, color3Loc;

float color1[] = { 1.0, 0.0, 0.0, 1.0 };
float color2[] = { 0.0, 1.0, 0.0, 1.0 };
float color3[] = { 0.0, 0.0, 1.0, 1.0 };

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 800, "Jam UTS", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    GLenum err = glewInit();

    float vertices[] = {
        1.0f, 1.0f,
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f
    };

    unsigned int indexArr[] = {
        0, 1, 2, 0, 2, 3
    };

    unsigned int vertexBuffer = 0;
    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexArr, GL_STATIC_DRAW);

    string vertexString = readFile("vertex.vert");
    string fragmentString = readFile("fragment.frag");

    const char* vertexChar = vertexString.c_str();
    const char* fragmentChar = fragmentString.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexChar, NULL);
    glShaderSource(fragmentShader, 1, &fragmentChar, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glUseProgram(program);

    GLint currentSecondPointerLoc = glGetUniformLocation(program, "currentSecondPointer");
    GLint currentMinutePointerLoc = glGetUniformLocation(program, "currentMinutePointer");
    GLint currentHourPointerLoc = glGetUniformLocation(program, "currentHourPointer");

    color1Loc = glGetUniformLocation(program, "color1");
    glUniform4f(color1Loc, color1[0], color1[1], color1[2], color1[3]);

    color2Loc = glGetUniformLocation(program, "color2");
    glUniform4f(color2Loc, color2[0], color2[1], color2[2], color2[3]);

    color3Loc = glGetUniformLocation(program, "color3");
    glUniform4f(color3Loc, color3[0], color3[1], color3[2], color3[3]);

    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        deltaTime = (currentTime - lastTime) * speed;
        lastTime = currentTime;

        currentSecondPointer += (deltaTime * velocitySecondPointer);
        currentMinutePointer += (deltaTime * velocityMinutePointer);
        currentHourPointer += (deltaTime * velocityHourPointer);

        glUniform1f(currentSecondPointerLoc, currentSecondPointer * 3.14159265f / 180.0f);
        glUniform1f(currentMinutePointerLoc, currentMinutePointer * 3.14159265f / 180.0f);
        glUniform1f(currentHourPointerLoc, currentHourPointer * 3.14159265f / 180.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        cout << "Kecepatan Jam Bertambah" << endl;
        speed += 4.0f;
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        cout << "Kecepatan Jam Seperti Semula" << endl;
        speed = 1.0f;
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        cout << "Warna Jam Terganti" << endl;
        int i = 0;
        while (i < 4)
        {
            color1[i] = (rand() * 1.0f) / RAND_MAX;
            color2[i] = (rand() * 1.0f) / RAND_MAX;
            color3[i] = (rand() * 1.0f) / RAND_MAX;
            i++;
        }

        glUniform4f(color1Loc, color1[0], color1[1], color1[2], color1[3]);
        glUniform4f(color2Loc, color2[0], color2[1], color2[2], color2[3]);
        glUniform4f(color3Loc, color3[0], color3[1], color3[2], color3[3]);
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        cout << "Menambah 1 Jam" << endl;
        glfwSetTime(currentTime + 3600);
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        cout << "Mengurangi 1 Jam" << endl;
        glfwSetTime(currentTime - 3600);
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        cout << "Menambah 1 Menit" << endl;
        glfwSetTime(currentTime + 60);
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        cout << "Mengurangi 1 Menit" << endl;
        glfwSetTime(currentTime - 60);
    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        float inputHour, inputMinute, inputSecond;
        cout << "Input Jam Menit dan Detik Yang Diinginkan" << endl;
        cout << "Jam : ";
        cin >> inputHour;

        cout << "Menit : ";
        cin >> inputMinute;

        cout << "Detik : ";
        cin >> inputSecond;

        float hour = inputHour * 3600;
        float minute = inputMinute * 60;
        float second = inputSecond;

        glfwSetTime(hour + minute + second);
        cout << "Jam DISESUAIKAN!" << endl;
    }
}
