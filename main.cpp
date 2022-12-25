#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "utilities.h"
#include "mesh.h"
#include "model.h"
#include <iostream>
#include <cmath>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, std::max(width, height), std::max(width, height));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const unsigned int size = 10;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "i use arch btw", NULL, NULL);

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    stbi_set_flip_vertically_on_load(true);

    float chessBoard[size][size][3];
    int threshold = 512;
    for (int indexx = 0; indexx < size; ++indexx) {
        for (int indexy = 0; indexy < size; ++indexy) {
            chessBoard[indexx][indexy][0] = indexx;
            chessBoard[indexx][indexy][2] = indexy;
            chessBoard[indexx][indexy][1] = indexy*indexx;
        }
    }

//    unsigned int indicesChess[511*511*2*3];
    std::vector<unsigned int> indicesChess;

//    indicesChess[0] = 1;
// change jstride of indexx
    for (unsigned int indexx = 0; indexx < (size-1)*(size-1); indexx += size) {
        for (unsigned int indexy = 0; indexy < size-1; ++indexy) {
            indicesChess.push_back(indexy + indexx);
            indicesChess.push_back(indexx + indexy + 1);
            indicesChess.push_back(indexx + indexy + size);
            indicesChess.push_back(indexx + indexy + 1);
            indicesChess.push_back(indexx + indexy + size);
            indicesChess.push_back(indexx + indexy + size + 1);
        }
    }

    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (size*size*3 * sizeof(float)), &chessBoard[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesChess.size() * sizeof(unsigned int), &indicesChess[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    Shader basicShader("basic.vert", "basic.frag");
    Shader modelShader("model.vert","model.frag");
    Model testModel(std::filesystem::path("backpack/backpack.obj").c_str());
    // Other initial data points for use in the loop
    const float speed = 2.0f;
    deltaTime = 0.0f;	// Time between current frame and last frame
    lastFrame = 0.0f;

    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::mat4 light_model = glm::mat4(1.0f);

    basicShader.use();

    //Usage
    while(!glfwWindowShouldClose(window)) {
        //set state here
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        view = camera.GetViewMatrix();

        processInput(window);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//            model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//            model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        //rendering stuff goes here
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        modelShader.use();
        basicShader.setMatrix("projection", projection);
        basicShader.setMatrix("view", view);
        basicShader.setMatrix("model", model);
        basicShader.setFloat("time",abs(glm::sin(currentFrame)));
//        testModel.Draw(modelShader);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indicesChess.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
