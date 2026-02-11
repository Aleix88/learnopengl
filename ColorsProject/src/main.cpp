#include <iostream>
#include <print>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include "cube.h"
#include "light.h"
#include "Helpers/window.hpp"
#include "Helpers/shader.hpp"
#include "Helpers/camera.hpp"

float W_WIDTH = 500.0f;
float W_HEIGHT = 500.0f;

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    W_WIDTH = (float)width;
    W_HEIGHT = (float)height;
    glViewport(0, 0, width, height);
}

Camera camera = Camera(
    glm::vec3(0.0f, 0.0f, 3.0f),
    0.0f,
    0.0f
);

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    camera.processMouseInput(xpos, ypos);
}

int main() {
    GLFWwindow* window = configureWindow("Amplitude", W_WIDTH, W_HEIGHT);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    loadOpenGLPointers();

    


    Cube cube = Cube(glm::vec3(1.0f, 0.0f, 0.0f), 0.3f);
    Light light = Light();

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);  

    while(!glfwWindowShouldClose(window)) {
        glClearColor(37/255.0f, 67/255.0f, 92/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cameraWASD(window, camera, 0.1f);

        cube.render(projectionMatrix, camera.viewMatrix());
        light.render(projectionMatrix, camera.viewMatrix());

        // SWAP AND POLL
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}