#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <cmath>

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "i use arch btw", NULL, NULL);
    float aspectRatio = 1280.0f/720;

    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 1280, 720);

    float vertices[] = {
            -0.5f/aspectRatio, 0, 0,
            0/aspectRatio, (float)sqrt(3)/2, 0,
            0.5f/aspectRatio, 0, 0
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Instructions for how to use the data given
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader ourShader("../vertShader.vert","../fragShader.frag");
    ourShader.use();

    while(!glfwWindowShouldClose(window)) {
        float timeValue = glfwGetTime();
        float colourG = (sin(timeValue) / 2.0f) + 0.5f;
        float colourR = (sin(timeValue + ((2*3.1415)/3)) / 2.0f) + 0.5f;
        float colourB = (sin(timeValue + ((4*3.1415)/3)) / 2.0f) + 0.5f;
        //rendering stuff goes here
        glClearColor(colourR, colourG, colourB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}