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

bool spacePressed = false;
void glfwKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        spacePressed = true;
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

int main() {
    GLFWwindow* window = configureWindow("Amplitude", W_WIDTH, W_HEIGHT);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
    glfwSetKeyCallback(window, glfwKeyCallBack);
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
    std::vector<Cube> tailCubes = {Cube(0.0f, 0.0f, 1.0f)};
    tailCubes[0].draw();
    tailCubes[0].bind(0);

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
    float snakeSpeed = 0.09f;
    std::vector<glm::vec3> snakePositions = {glm::vec3(planeLeft, 0.01f, planeNear)};
    
    struct DirectionChange {
        glm::vec3 direction;
        glm::vec3 position;
    };

    std::vector<DirectionChange> directionChanges;
    DirectionChange initialDirection;
    initialDirection.direction = snakeDirection;
    initialDirection.position = snakePositions[0];
    directionChanges.push_back(initialDirection);

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
            DirectionChange dc;
            dc.direction = snakeDirection;
            dc.position = snakePositions[0];
            directionChanges.push_back(dc);

        if (spacePressed) {
            int prevCube = snakePositions.size() - 1;
            tailCubes.push_back(Cube(0.0f, sin(glfwGetTime()), 0.0f));
            tailCubes[tailCubes.size() - 1].draw();
            glm::vec3 cubePosition = glm::vec3(
                snakePositions[prevCube].x - (snakeDirection.x * cubeSize),
                snakePositions[prevCube].y,
                snakePositions[prevCube].z - (snakeDirection.z * cubeSize)
            );
            snakePositions.push_back(cubePosition);
            std::println("{0}, {1}", snakePositions[prevCube].x, cubePosition.x);
            spacePressed = false;
        }

        for (int i = 0; i < tailCubes.size(); i++) {
            if (snakePositions[i].x < planeLeft) {
                snakePositions[i].x = planeRight - cubeSize;
            } else if (snakePositions[i].x + cubeSize > planeRight) {
                snakePositions[i].x = planeLeft;
            } else if (snakePositions[i].z > planeNear) {
                snakePositions[i].z = planeFar + cubeSize;
            } else if (snakePositions[i].z - cubeSize < planeFar) {
                snakePositions[i].z = planeNear;
            }

            snakeModelMatrix = glm::mat4(1);
            snakeModelMatrix[0][0] = 1;
            snakeModelMatrix[3][0] = snakePositions[i].x;
            snakeModelMatrix[2][2] = 1;
            snakeModelMatrix[3][2] = snakePositions[i].z;
            
            tailCubes[i].bind(0);
            shader.setMat4("modelMatrix", snakeModelMatrix);

            glDrawElements(GL_TRIANGLES, 36,  GL_UNSIGNED_INT, 0);

            glm::vec3 translation = snakeDirection * snakeSpeed * deltaTime;
            snakePositions[i] = snakePositions[i] + translation;
        }

        // SWAP AND POLL
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < tailCubes.size(); i++) {
        glDeleteBuffers(1, &tailCubes[i].VBO);
        glDeleteBuffers(1, &tailCubes[i].EBO);
    }

    return EXIT_SUCCESS;
}