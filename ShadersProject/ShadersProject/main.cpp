#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void configureGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

GLFWwindow* createWindow(int height, int width) {
    GLFWwindow* window = glfwCreateWindow(height, width, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::runtime_error("Failed to initialize GLAD");
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void loadOpenGLPointers() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::runtime_error("Failed to initialize GLAD");
    }
}

void run(GLFWwindow* window);

int main(int argc, const char * argv[]) {
    configureGLFW();
    try {
        GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT);
        loadOpenGLPointers();
        run(window);
    } catch(const std::exception& e) {
        glfwTerminate();
        return -1;
    }

    return EXIT_SUCCESS;
}

void run(GLFWwindow* window) {
    // By default xcode use DerivedData folder as the working directory.
    // So in order to read the files you need to edit the Working direcotry from
    // "Edit scheme" -> "Run" -> "Options" -> "Working directory"
    Shader shader = Shader("vertexShader.vs", "fragmentShader.fs");

    // -- Configure shape --
    float vertexs[] = {
        // Vertex           // Colors
         0.0f,  0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,1.0f,
         0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,
    };

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

    // Two options declar the location of the attribute in the shader using `layout(location = 0) in...`
    // or use glGetAttribLocation to find the location of the attribute (avoiding the layout(locaition = 0))
    int attributeLocation = glGetAttribLocation(shader.ID, "aPos");
    glVertexAttribPointer(attributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)0);
    glEnableVertexAttribArray(attributeLocation);

    int colorAttribute = glGetAttribLocation(shader.ID, "myColor");
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(colorAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.1f,0.2f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.ID);
    glfwTerminate();
}
