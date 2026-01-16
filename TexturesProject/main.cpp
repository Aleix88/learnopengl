#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

void setFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void loadOpenGLPointers() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::runtime_error("Failed to initialize GLAD");
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "Textures project", NULL, NULL);
    if (window == NULL) {
        std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, setFramebufferSizeCallback);

    loadOpenGLPointers();

    float vertex[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };

    Shader shader = Shader("./vertexShader.vs", "./fragmentShader.fs");

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // https://stackoverflow.com/questions/37972229/glvertexattribpointer-and-glvertexattribformat-whats-the-difference
    // Esquema -> https://stackoverflow.com/questions/63981260/vertex-buffer-binding-index-and-uniform-buffer-binding-points
    // glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    // glBindVertexBuffer(0, VBO, 0, sizeof(float) * 3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // SECOND VAO
    float vertex2[] = {
        0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };
    unsigned int VAO2, VBO2;

    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex2), vertex2, GL_STATIC_DRAW);

    // https://stackoverflow.com/questions/37972229/glvertexattribpointer-and-glvertexattribformat-whats-the-difference
    // Esquema -> https://stackoverflow.com/questions/63981260/vertex-buffer-binding-index-and-uniform-buffer-binding-points
    // THIS WONT WORK BECAUSE IN MAC ARM ARCHITECTURE 4.1 openGL VERSION IS THE LAST SUPPORTED    
    // Binding the shader attribute to the vertex binding index
    // glVertexAttribBinding(0, 0);
    // Specify the attribute index format data
    // glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    // glEnableVertexAttribArray(0);
    // Bind the VBO to the vertex binding index
    // glBindVertexBuffer(0, VBO2, 0, sizeof(float) * 3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.4f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        if (glfwGetTime() > 3) {
            glBindVertexArray(VAO2);
        } else {
            glBindVertexArray(VAO);
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}