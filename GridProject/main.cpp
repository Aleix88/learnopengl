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

float W_WIDTH = 500.0f;
float W_HEIGHT = 500.0f;

Camera camera = Camera(
    glm::vec3(0.0f, 2.3f, 0.4f), 
    -42, // PITCH
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

float cubeSize = 0.1f;
float size = cubeSize*20;
float right = size/2;
float left = -right;
float near = -1.0f;
float far = near - size;

unsigned int createGroundVBO() {
    float vertices[] = {
        // Position           // Color
        left, 0.0f, near,     1.0f, 0.0f, 0.0f, 1.0f, // TOP-LEFT
        right, 0.0f, near,    1.0f, 0.0f, 0.0f, 1.0f, // TOP-RIGHT
        left, 0.0f, far,      1.0f, 0.0f, 0.0f, 1.0f, // BOTTOM-LEFT
        right, 0.0f, far,     1.0f, 0.0f, 0.0f, 1.0f, // BOTTOM-RIGHT
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
    std::vector<float> vertices; 
    std::vector<unsigned int> elements;


    for (int r = 0; r < size/cubeSize; r++) {
        for (int c = 0; c < size/cubeSize; c++) {   
            float newLeft = left + cubeSize * c;
            float newNear = near - cubeSize * r;
            std::vector<float> newVertices = {
                // Position                              // Color
                newLeft, y, newNear - cubeSize,                0.0f, 0.0f, 1.0f, alpha,
                newLeft, y, newNear,                           0.0f, 0.0f, 1.0f, alpha,
                newLeft + cubeSize, y, newNear,                0.0f, 0.0f, 1.0f, alpha,
                newLeft + cubeSize, y, newNear - cubeSize,     0.0f, 0.0f, 1.0f, alpha,
    
                newLeft, y + cubeSize, newNear - cubeSize,                0.0f, 0.0f, 1.0f, alpha,
                newLeft, y + cubeSize, newNear,                           0.0f, 0.0f, 1.0f, alpha,
                newLeft + cubeSize, y + cubeSize, newNear,                0.0f, 0.0f, 1.0f, alpha,
                newLeft + cubeSize, y + cubeSize, newNear - cubeSize,     0.0f, 0.0f, 1.0f, alpha,
            };
            vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
    
            std::vector newElements = {0,1,2, 2,3,0, 4,5,6, 6,7,4};
            for (int i = 0; i < 4; i++) {
                int next = (i + 1) % 4;
                // Primer triangulo de la cara
                newElements.push_back(i);
                newElements.push_back(i+4);
                newElements.push_back(next);
                // Segundo triangulo de la cara
                newElements.push_back(next);
                newElements.push_back(i+4);
                newElements.push_back(next+4);
            }
            for (int i = 0; i < newElements.size(); i++) {
                newElements[i] = newElements[i] + (c + 20 * r) * 8;
            }
            elements.insert(elements.end(), newElements.begin(), newElements.end());
        }
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
    unsigned int cubesVBO = createCubesVBO();
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
    glm::mat4 projectionMatrix = glm::mat4(1);

    projectionMatrix = glm::perspective(glm::radians(45.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("modelMatrix", modelMatrix);
    shader.setMat4("projectionMatrix", projectionMatrix);

    glEnable(GL_DEPTH_TEST);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    float lastTime = 0.0f;
    while(!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // cameraWASD(window, camera, deltaTime);

        shader.setMat4("viewMatrix", camera.viewMatrix());

        glBindVertexArray(groundVAO);
        glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_INT, 0);
        glBindVertexArray(cubesVAO);
        glDrawElements(GL_TRIANGLES, 36 * 20 * 20,  GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}