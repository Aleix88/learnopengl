#include <iostream>
#include <print>
#include <vector>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "libs/stb_image.h"
#include "Helpers/shader.hpp"
#include "Helpers/camera.hpp"
#include "Helpers/window.hpp"
#include "SceneObjects/snake.h"
#include "SceneObjects/ground.h"

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

int main() {
    GLFWwindow* window = configureWindow("Amplitude", W_WIDTH, W_HEIGHT);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
    glfwSetKeyCallback(window, glfwKeyCallBack);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    loadOpenGLPointers();
    
    Shader shader = Shader("src/Shaders/vertexShader.glsl", "src/Shaders/fragmentShader.glsl");
    shader.use();

    Ground ground = Ground(&shader);
    Snake snake = Snake(&shader);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projectionMatrix", projectionMatrix);

    glEnable(GL_DEPTH_TEST);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setMat4("viewMatrix", camera.viewMatrix());
        // cameraWASD(window, camera, deltaTime);

        ground.render();
        snake.render(keyPressed);

        keyPressed = -1;

        // SWAP AND POLL
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}