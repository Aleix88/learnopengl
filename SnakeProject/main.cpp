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

int keyPressed;
void glfwKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    keyPressed = action == GLFW_PRESS ? key : -1;
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    
    // Position attribute
    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribBinding(1, 0);
    glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);

    Shader shader = Shader("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
    shader.use();

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projectionMatrix", projectionMatrix);

    glEnable(GL_DEPTH_TEST);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    std::vector<glm::ivec3> snakeDirections = {
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
    };
    // Hacer esto con ints porque el float da problemas de precision
    std::vector<glm::ivec3> snakePositions = {
        glm::vec3(0, 2, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 0),
    };
    std::vector<Cube*> cubes = {
        new Cube(0.0f, 1.0f, 0.0f),
        new Cube(0.0f, 1.0f, 0.0f),
        new Cube(0.0f, 1.0f, 0.0f),
    };
    glm::ivec3 nextDirection = snakeDirections[0];

    float lastTime = 0.0f;
    float lastAddTimestamp = 0.0f;
    while(!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // cameraWASD(window, camera, deltaTime);
        shader.setMat4("viewMatrix", camera.viewMatrix());

        // DRAW GROUND
        glBindVertexArray(groundVAO);
        // Hauriem de crear un altre shader per no tenir que setejar a 0 aixo pel terra
        shader.setVec2("posOffset", glm::vec2(0));
        glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_INT, 0);

        // DRAW SNAKE
        glBindVertexArray(cubesVAO);

        if (keyPressed == GLFW_KEY_W) {
            nextDirection = glm::vec3(0,0,1);
        }
        if (keyPressed == GLFW_KEY_S) {
            nextDirection = glm::vec3(0,0,-1);
        }
        if (keyPressed == GLFW_KEY_A) {
            nextDirection = glm::vec3(-1,0,0);
        }
        if (keyPressed == GLFW_KEY_D) {
            nextDirection = glm::vec3(1,0,0);
        }

        if (nextDirection != -snakeDirections[0]) {
            snakeDirections[0] = nextDirection;
        }

        if (keyPressed == GLFW_KEY_SPACE) {
            cubes.push_back(new Cube(0,1.0,0));
            (*(cubes.end() - 1))->draw();
            glm::vec3 prevPosition = *(snakePositions.end() - 1);
            glm::vec3 prevDirection = *(snakeDirections.end() - 1);
            glm::vec3 newElementPos = prevPosition - cubeSize * prevDirection;
            snakePositions.push_back(newElementPos);
            snakeDirections.push_back(prevDirection);
        }
        keyPressed = -1;

        if (glfwGetTime() - lastAddTimestamp > 0.1f) {
            glm::ivec3 newHeadCellPos = snakePositions[0] + snakeDirections[0];
            std::println("X: {0}, Y: {1}", newHeadCellPos.x, newHeadCellPos.z);
            if (newHeadCellPos.x > 19) {
                newHeadCellPos.x = 0;
            } else if (newHeadCellPos.x < 0) {
                newHeadCellPos.x = 19;
            } else if (newHeadCellPos.z > 19) {
                newHeadCellPos.z = 0;
            } else if (newHeadCellPos.z < 0) {
                newHeadCellPos.z = 19;
            }

            for (int i = snakePositions.size() - 1; i >= 0; i--) {
                if (i == 0) {
                    snakePositions[i] = newHeadCellPos;
                } else {
                    snakeDirections[i] = snakeDirections[i-1];
                    snakePositions[i] = snakePositions[i-1];
                    if (snakePositions[i] == newHeadCellPos) {
                        std::println("You losed you dum dum...");
                        glfwSetWindowShouldClose(window, true);
                        break;
                    }
                }
                lastAddTimestamp = glfwGetTime();
            }
        }
        
        for (int i = 0; i < snakePositions.size(); i++) {
            glm::vec3 offset = glm::vec3(
                snakePositions[i].x * cubeSize, 
                0, 
                -snakePositions[i].z * cubeSize
            );
            shader.setVec2("posOffset", glm::vec2(offset.x, offset.z));
            (cubes[i])->bind(0);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        // SWAP AND POLL
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}