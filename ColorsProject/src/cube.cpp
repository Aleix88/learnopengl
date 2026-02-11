#include <iostream>
#include <print>
#include "cube.h"

Cube::Cube(glm::vec3 color, float size):shader(createShader()) {
    this->color = color;
    this->cubeSize = size;
    vertices = createCubeVertices();
    elements = createCubeElements();
    draw();
};

Cube::~Cube() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cube::draw() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Positions
    glVertexAttribBinding(0,0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribBinding(1,0);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindVertexBuffer(0, VBO, 0, sizeof(float) * 6);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), elements.data(), GL_STATIC_DRAW);
}

Shader Cube::createShader() {
    return Shader("src/Shaders/cubeVertex.glsl", "src/Shaders/cubeFragment.glsl", ::FILE_PATH);
}

void Cube::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    shader.use();
    shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    shader.setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setMat4("viewMatrix", viewMatrix);
    shader.setMat4("modelMatrix", glm::mat4(1));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

std::vector<float> Cube::createCubeVertices() {
    float y = 0.1f;
    glm::vec3 gradientColor = color - 0.3f;
    std::vector<float> vertices = {
        // Position                                  // Color
        0, y, -cubeSize,                             gradientColor.r, gradientColor.g, gradientColor.b,
        0, y, 0,                                     gradientColor.r, gradientColor.g, gradientColor.b,
        cubeSize, y, 0,                              gradientColor.r, gradientColor.g, gradientColor.b,
        cubeSize, y, -cubeSize,                      gradientColor.r, gradientColor.g, gradientColor.b,

        0, y + cubeSize, -cubeSize,                  color.r, color.g, color.b,
        0, y + cubeSize, 0,                          color.r, color.g, color.b,
        0 + cubeSize, y + cubeSize, 0,               color.r, color.g, color.b,
        0 + cubeSize, y + cubeSize, -cubeSize,       color.r, color.g, color.b,
    };
    return vertices;
}

std::vector<unsigned int> Cube::createCubeElements() {
    std::vector<unsigned int> elements = {0,1,2, 2,3,0, 4,5,6, 6,7,4};
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        // Primer triangulo de la cara
        elements.push_back(i);
        elements.push_back(i+4);
        elements.push_back(next);
        // Segundo triangulo de la cara
        elements.push_back(next);
        elements.push_back(i+4);
        elements.push_back(next+4);
    }
    return elements;
}