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
#include "SceneObjects/food.h"

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
    
    Ground ground = Ground();
    Snake snake = Snake();
    Food food = Food();
    
    glEnable(GL_DEPTH_TEST);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);
    
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // cameraWASD(window, camera, 1.0f);

        ground.render(projectionMatrix, camera.viewMatrix());
        snake.render(keyPressed, projectionMatrix, camera.viewMatrix(), food.foodCell);
        food.render(projectionMatrix, camera.viewMatrix());

        if (snake.collisionDetected) {
            food.deleteFood();
            snake.collisionDetected = false;
        }

        if (snake.snakeDied) {
            glfwSetWindowShouldClose(window, true);
        }

        keyPressed = -1;

        // SWAP AND POLL
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}