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

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

const float W_WIDTH = 500.0f;
const float W_HEIGHT = 500.0f;

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

    loadOpenGLPointers();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

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
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    unsigned int VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribBinding(1, 0);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);

    glBindVertexBuffer(0, VBO, 0, sizeof(float) * 5);

    glBindVertexArray(0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Shader shader = Shader("vertexShader.glsl", "fragmentShader.glsl");
    shader.use();

    unsigned int boxTexture = loadTexture("Textures/container.jpg", 0);
    unsigned int faceTexture = loadTexture("Textures/awesomeface.png", 1);
    glUniform1i(glGetUniformLocation(shader.ID, "boxTexture"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "faceTexture"), 1);

    glEnable(GL_DEPTH_TEST);

    float near = 1.0f;
    const float far = 100.0f;
    float leftToRight = 10.0f;
    float fov = 2 * atan((leftToRight/2)/near);

    float cameraYRotation = 0.0f;

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // CODE TO PLAY WITH THE NEAR PLANE WIDTH
        // if (glfwGetKey(window, GLFW_KEY_W)) {
        //     leftToRight += 0.1f;
        // } else if (glfwGetKey(window, GLFW_KEY_S)) {
        //     leftToRight -= 0.1f;
        // }  
        
        if (glfwGetKey(window, GLFW_KEY_A)) {
            cameraYRotation += 0.1f;
        } else if (glfwGetKey(window, GLFW_KEY_D)) {
            cameraYRotation -= 0.1f;
        }
        fov = 2 * atan((leftToRight/2)/near);
        std::println("Width: {0}  -  FOV: {1}º", leftToRight, glm::degrees(fov));

        glm::mat4x4 projectionMatrix = glm::mat4x4(1);
        projectionMatrix = glm::perspective(fov, W_WIDTH/W_HEIGHT, near, far);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "vProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glm::mat4x4 viewMatrix = glm::mat4x4(1);
        // T * R
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
        viewMatrix = glm::rotate(viewMatrix, cameraYRotation, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "vViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

        glBindVertexArray(VAO);

        for (int i = 0; i < sizeof(cubePositions)/(sizeof(*cubePositions)); i++) {
            glm::mat4x4 modelMatrix = glm::mat4x4(1);
            // T * R
            // if (i == 0) {
            //     modelMatrix = glm::translate(modelMatrix, glm::vec3(sin((float)glfwGetTime()), cubePositions[i].y, cubePositions[i].z));
            // } else {
                modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            // }
            // modelMatrix = glm::rotate(modelMatrix, (glm::radians(45.0f) * (float)glfwGetTime()), glm::vec3(1.0f,0.0f,1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "vModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}