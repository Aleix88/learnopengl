#include <iostream>
#include <print>
#include <vector>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Helpers/shader.hpp"
#include "Helpers/camera.hpp"
#include "Helpers/texture.hpp"
#include "Helpers/window.hpp"
#include "constants.h"
#include "cube.h"

float W_WIDTH = 500.0f;
float W_HEIGHT = 500.0f;

Camera camera = Camera(
    glm::vec3(1.0f, 2.0f, 1.8f), 
    -32, // PITCH
    0 // YAW
);

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    W_WIDTH = (float)width;
    W_HEIGHT = (float)height;
    glViewport(0, 0, width, height);
}

void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    // camera.processMouseInput(xpos, ypos);
}

unsigned int createGroundVBO() {
    float vertices[] = {
        // Position           // Color
        planeLeft, 0.0f, planeNear,     1.0f, 0.0f, 0.0f, 1.0f, // TOP-LEFT
        planeRight, 0.0f, planeNear,    1.0f, 0.0f, 0.0f, 1.0f, // TOP-RIGHT
        planeLeft, 0.0f, planeFar,      1.0f, 0.0f, 0.0f, 1.0f, // BOTTOM-LEFT
        planeRight, 0.0f, planeFar,     1.0f, 0.0f, 0.0f, 1.0f, // BOTTOM-RIGHT
    };

    unsigned int elements[] = {
        0, 1, 2,
        1, 2, 3,
    };

    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexBuffer(0, vbo, 0, sizeof(float) * 7);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    return vbo;
}

unsigned int createCubesVBO() {
    float alpha = 0.4f;
    float y = 0.01f;
    std::vector<float> vertices = {
        // Position                              // Color
        planeLeft, y, planeNear - cubeSize,                0.0f, 0.0f, 1.0f, alpha,
        planeLeft, y, planeNear,                           0.0f, 0.0f, 1.0f, alpha,
        planeLeft + cubeSize, y, planeNear,                0.0f, 0.0f, 1.0f, alpha,
        planeLeft + cubeSize, y, planeNear - cubeSize,     0.0f, 0.0f, 1.0f, alpha,

        planeLeft, y + cubeSize, planeNear - cubeSize,                0.0f, 0.0f, 1.0f, alpha,
        planeLeft, y + cubeSize, planeNear,                           0.0f, 0.0f, 1.0f, alpha,
        planeLeft + cubeSize, y + cubeSize, planeNear,                0.0f, 0.0f, 1.0f, alpha,
        planeLeft + cubeSize, y + cubeSize, planeNear - cubeSize,     0.0f, 0.0f, 1.0f, alpha,
    };

    std::vector elements = {0,1,2, 2,3,0, 4,5,6, 6,7,4};
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        // Primer triangulo de la cara
        elements.push_back(i);
        elements.push_back(i+4);
        elements.push_back(next);
        // Segundo triangulo de la cara
        elements.push_back(next);
        elements.push_back(i+4);
        elements.push_back(next+4);
    }

    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindVertexBuffer(0, vbo, 0, sizeof(float) * 7);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), &elements[0], GL_STATIC_DRAW);

    return vbo;
}

int main() {
    GLFWwindow* window = configureWindow("Amplitude", W_WIDTH, W_HEIGHT);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    loadOpenGLPointers();

    unsigned int groundVAO;
    glGenVertexArrays(1, &groundVAO);
    glBindVertexArray(groundVAO);
    unsigned int groundVBO = createGroundVBO();
    // Position attribute
    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribBinding(1, 0);
    glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);

    unsigned int cubesVAO;
    glGenVertexArrays(1, &cubesVAO);
    glBindVertexArray(cubesVAO);
    // unsigned int cubesVBO = createCubesVBO();
    Cube cube = Cube();
    cube.draw();
    cube.bind(0);

    // Position attribute
    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribBinding(1, 0);
    glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Shader shader = Shader("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
    shader.use();

    glm::mat4 modelMatrix = glm::mat4(1);
    glm::mat4 snakeModelMatrix = glm::mat4(1);
    glm::mat4 projectionMatrix = glm::mat4(1);

    projectionMatrix = glm::perspective(glm::radians(45.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("modelMatrix", modelMatrix);
    shader.setMat4("projectionMatrix", projectionMatrix);

    glEnable(GL_DEPTH_TEST);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    glm::vec3 snakeDirection = glm::vec3(1.0f,0.0f,0.0f);
    float snakeSpeed = 0.8f;
    glm::vec3 snakePosition = glm::vec3(planeLeft, 0.01f, planeNear);

    float lastTime = 0.0f;    
    while(!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // cameraWASD(window, camera, deltaTime);
        shader.setMat4("viewMatrix", camera.viewMatrix());

        // DRAW GROUND
        glBindVertexArray(groundVAO);
        shader.setMat4("modelMatrix", modelMatrix);
        glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_INT, 0);

        // DRAW SNAKE
        glBindVertexArray(cubesVAO);

        if (glfwGetKey(window, GLFW_KEY_W))
            snakeDirection = glm::vec3(0,0,-1);
        if (glfwGetKey(window, GLFW_KEY_S))
            snakeDirection = glm::vec3(0,0,1);
        if (glfwGetKey(window, GLFW_KEY_A))
            snakeDirection = glm::vec3(-1,0,0);
        if (glfwGetKey(window, GLFW_KEY_D))
            snakeDirection = glm::vec3(1,0,0);

        glm::vec3 translation = snakeDirection * snakeSpeed * deltaTime;
        snakePosition += translation;
        // std::println("{0} {1} {2}", snakePosition.x, snakePosition.y, snakePosition.z);
        if (snakePosition.x < planeLeft) {
            snakePosition.x = planeRight - cubeSize;
        } else if (snakePosition.x + cubeSize > planeRight) {
            snakePosition.x = planeLeft;
        } else if (snakePosition.z > planeNear) {
            snakePosition.z = planeFar + cubeSize;
        } else if (snakePosition.z - cubeSize < planeFar) {
            snakePosition.z = planeNear;
        }

        // Go back to identity matrix but with a custom position
        snakeModelMatrix[0][0] = 1;
        snakeModelMatrix[3][0] = snakePosition.x;
        snakeModelMatrix[2][2] = 1;
        snakeModelMatrix[3][2] = snakePosition.z;

        snakeModelMatrix = glm::translate(snakeModelMatrix, translation);
        shader.setMat4("modelMatrix", snakeModelMatrix);
        glDrawElements(GL_TRIANGLES, 36,  GL_UNSIGNED_INT, 0);


        // SWAP AND POLL
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}