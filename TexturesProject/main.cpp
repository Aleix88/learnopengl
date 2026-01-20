#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void setFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void loadOpenGLPointers() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::runtime_error("Failed to initialize GLAD");
    }
}

unsigned int makeTexture() {
    // Create and bind the texture
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // Configure texture behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Load and generate the texture based on the image
    int tWidth, tHeight, nChannels;
    unsigned char* data = stbi_load("Textures/container.jpg", &tWidth, &tHeight, &nChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Error loading texture..." << std::endl;
    }
    stbi_image_free(data);
    return textureId;
}

unsigned int makeAnotherTexture() {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("./Textures/awesomeface.png", &width, &height, &nChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Error loading awesome image" << std::endl;
    }
    stbi_image_free(data);
    return textureId;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "Textures project", NULL, NULL);
    if (window == NULL) {
        std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, setFramebufferSizeCallback);

    loadOpenGLPointers();

    Shader shader = Shader("./vertexShader.glsl", "./fragmentShader.glsl");

    float vertex[] = {
        // POSITIONS          // COLORS             // TEXTURES
        -0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,     // TOP-LEFT
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,     // BOTTOM-LEFT
        0.5f, 0.5f, 0.0f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f,     // TOP-RIGHT
        0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,     // BOTTOM-RIGHT
    };
    unsigned int elements[] = {
        0, 1, 2,
        1, 2, 3
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

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

    // Position attribute (0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    // Colors attribute (1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);

    // Textures attribute (2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Configure texture
    unsigned int textureId = makeTexture();
    unsigned int anotherTextureId = makeAnotherTexture();

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 3);
    glUniform1i(glGetUniformLocation(shader.ID, "anotherTexture"), 0);
    float mixValue = 0.5;

    while (!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            mixValue += 0.01;
            mixValue = mixValue > 1 ? 1 : mixValue;
        } else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            mixValue -= 0.01;
            mixValue = mixValue < 0 ? 0 : mixValue;
        }

        glClearColor(0.1f, 0.4f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ALL OF THESE IS ADDED IN THE TUTORIAL, BUT IT SEEMS ITS NOT NEEDED...
        // glActiveTexture(GL_TEXTURE3);
        // glBindTexture(GL_TEXTURE_2D, textureId);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, anotherTextureId);
        // shader.use();

        glUniform1f(glGetUniformLocation(shader.ID, "mixValue"), mixValue);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}