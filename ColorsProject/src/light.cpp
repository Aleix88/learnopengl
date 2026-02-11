#include <iostream>
#include <print>
#include "light.h"

Light::Light():shader(createShader()) {
    vertices = createCubeVertices();
    elements = createCubeElements();
    draw();
};

Light::~Light() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Light::draw() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Positions
    glVertexAttribBinding(0,0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindVertexBuffer(0, VBO, 0, sizeof(float) * 3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), elements.data(), GL_STATIC_DRAW);
}

Shader Light::createShader() {
    return Shader("src/Shaders/lightVertex.glsl", "src/Shaders/lightFragment.glsl", ::FILE_PATH);
}

void Light::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    shader.use();
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setMat4("viewMatrix", viewMatrix);
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("modelMatrix", modelMatrix);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

std::vector<float> Light::createCubeVertices() {
    std::vector<float> vertices = {
        // Position                                
        0, 0, -cubeSize, 
        0, 0, 0,                             
        cubeSize, 0, 0,
        cubeSize, 0, -cubeSize,

        0, cubeSize, -cubeSize,
        0, cubeSize, 0,
        cubeSize, cubeSize, 0,
        cubeSize, cubeSize, -cubeSize,
    };
    return vertices;
}

std::vector<unsigned int> Light::createCubeElements() {
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