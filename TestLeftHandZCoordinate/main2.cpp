#include <iostream>
#include <print>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.hpp"

float W_WIDTH = 500.0f;
float W_HEIGHT = 500.0f;
int W_PIXEL_WIDTH = 0;
int W_PIXEL_HEIGHT = 0;

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    W_WIDTH = (float)width;
    W_HEIGHT = (float)height;
    glViewport(0, 0, width, height);
}

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
    W_PIXEL_WIDTH = width;
    W_PIXEL_HEIGHT = height;
}

void loadOpenGLPointers() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::runtime_error("Failed to initialize GLAD");
    }
}

unsigned int loadTexture(std::string source, int index) {
    unsigned int id;
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // OPENGL stores textures in memory bottom up, the first byte of the image is the top-left
    // But the UV coordinates reads the image starting at bottom-left. Thats why the image must 
    // be flipped when loading
    // https://www.puredevsoftware.com/blog/2018/03/17/texture-coordinates-d3d-vs-opengl/

    bool isPNG = source.find(".png") != std::string::npos;
    GLenum imageType = isPNG ? GL_RGBA : GL_RGB;

    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(source.c_str(), &width, &height, &nChannels, 0);
    if (data == NULL) {
        std::println("Error loading image");
        return -1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageType, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return id;
}

float lastXPos = 0.0f;
float lastYPos = 0.0f;
float cameraRotationY = 0.0f;
float cameraRotationX = 0.0f;
glm::vec3 cameraDirection;

void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    float centerXPos = xpos - W_PIXEL_WIDTH/2;
    float centerYPos = ypos - W_PIXEL_HEIGHT/2;
    float deltaX = centerXPos - lastXPos;
    float deltaY = centerYPos - lastYPos;
    const float rotationIncrement = 0.01f;
    cameraRotationX += rotationIncrement * deltaY;
    cameraRotationY += rotationIncrement * deltaX;

    cameraDirection = glm::vec3
    (
        cos(cameraRotationY),
        -cos(cameraRotationX),
        sin(cameraRotationY) + sin(cameraRotationX)
    );
    cameraDirection = glm::normalize(cameraDirection);

    lastXPos = centerXPos;
    lastYPos = centerYPos;
}

void printVector(std::string name, glm::vec3 v) {
    std::println("{0}: ({1}, {2}, {3}) ", name, v.x, v.y, v.z);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Arch window", NULL, NULL);
    if (window == NULL) {
        std::println("Error creating window!");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    
    loadOpenGLPointers();

    float vertices[] = {
        -0.5f, -0.5f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  -0.0f), 
        // glm::vec3( 2.0f,  5.0f, -15.0f), 
        // glm::vec3(-1.5f, -2.2f, -2.5f),  
        // glm::vec3(-3.8f, -2.0f, -12.3f),  
        // glm::vec3( 2.4f, -0.4f, -3.5f),  
        // glm::vec3(-1.7f,  3.0f, -7.5f),  
        // glm::vec3( 1.3f, -2.0f, -2.5f),  
        // glm::vec3( 1.5f,  2.0f, -2.5f), 
        // glm::vec3( 1.5f,  0.2f, -1.5f), 
        // glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    unsigned int VAO, VBO;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribBinding(1, 0);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);

    glBindVertexBuffer(0, VBO, 0, sizeof(float) * 5);

    glBindVertexArray(0);

    Shader shader = Shader("vertexShader.glsl", "fragmentShader.glsl");
    shader.use();

    unsigned int boxTexture = loadTexture("Textures/container.jpg", 0);
    unsigned int faceTexture = loadTexture("Textures/awesomeface.png", 1);
    glUniform1i(glGetUniformLocation(shader.ID, "boxTexture"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "faceTexture"), 1);

    glEnable(GL_DEPTH_TEST);

    glm::mat4x4 projectionMatrix = glm::mat4x4(1);
    projectionMatrix = glm::perspective(glm::radians(80.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "vProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float lastFrameTime = 0.0f;
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float deltaTime = glfwGetTime() - lastFrameTime;
        lastFrameTime = glfwGetTime();

        glm::mat4x4 viewMatrix = glm::mat4x4(1);
        float cameraSpeed = 3.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W))
            cameraPos += cameraSpeed * cameraForward;
        else if(glfwGetKey(window, GLFW_KEY_S))
            cameraPos -= cameraSpeed * cameraForward;
        else if(glfwGetKey(window, GLFW_KEY_A))
            cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraForward, cameraUp));
        else if(glfwGetKey(window, GLFW_KEY_D))
            cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraForward, cameraUp));

        viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "vViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

        glBindVertexArray(VAO);

        for (int i = 0; i < sizeof(cubePositions)/(sizeof(*cubePositions)); i++) {
            glm::mat4x4 modelMatrix = glm::mat4x4(1);
            // modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "vModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            // glDrawArrays(GL_TRIANGLES, 0, 36);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}