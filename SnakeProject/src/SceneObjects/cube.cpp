#include <iostream>
#include <print>
#include "constants.h"
#include "cube.h"

Cube::Cube(glm::vec3 color) {
    this->color = color;
    vertices = createCubeVertices();
    elements = createCubeElements();
    draw();
};

Cube::~Cube() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Moure el elements array d'aqui y afegiro al snake que es on es crea el VAO.
void Cube::draw() {
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), elements.data(), GL_STATIC_DRAW);
}

void Cube::bind(unsigned int index) {
    glBindVertexBuffer(index, VBO, 0, sizeof(float) * 7);
}

std::vector<float> Cube::createCubeVertices() {
    float alpha = 1.0f;
    float y = 0.01f;
    glm::vec3 gradientColor = color - 0.3f;
    std::vector<float> vertices = {
        // Position                                  // Color
        0, y, -cubeSize,                             gradientColor.r, gradientColor.g, gradientColor.b, alpha,
        0, y, 0,                                     gradientColor.r, gradientColor.g, gradientColor.b, alpha,
        cubeSize, y, 0,                              gradientColor.r, gradientColor.g, gradientColor.b, alpha,
        cubeSize, y, -cubeSize,                      gradientColor.r, gradientColor.g, gradientColor.b, alpha,

        0, y + cubeSize, -cubeSize,                  color.r, color.g, color.b, alpha,
        0, y + cubeSize, 0,                          color.r, color.g, color.b, alpha,
        0 + cubeSize, y + cubeSize, 0,               color.r, color.g, color.b, alpha,
        0 + cubeSize, y + cubeSize, -cubeSize,       color.r, color.g, color.b, alpha,
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