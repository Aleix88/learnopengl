#include <iostream>
#include <glad/glad.h>
#include "constants.h"
#include "cube.h"

Cube::Cube() {
    vertices = createCubeVertices();
    elements = createCubeElements();
};

Cube::~Cube() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

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
    float alpha = 0.4f;
    float y = 0.01f;
    std::vector<float> vertices = {
        // Position                                                     // Color
        planeLeft, y, planeNear - cubeSize,                             0.0f, 0.0f, 1.0f, alpha,
        planeLeft, y, planeNear,                                        0.0f, 0.0f, 1.0f, alpha,
        planeLeft + cubeSize, y, planeNear,                             0.0f, 0.0f, 1.0f, alpha,
        planeLeft + cubeSize, y, planeNear - cubeSize,                  0.0f, 0.0f, 1.0f, alpha,

        planeLeft, y + cubeSize, planeNear - cubeSize,                  0.0f, 0.0f, 1.0f, alpha,
        planeLeft, y + cubeSize, planeNear,                             0.0f, 0.0f, 1.0f, alpha,
        planeLeft + cubeSize, y + cubeSize, planeNear,                  0.0f, 0.0f, 1.0f, alpha,
        planeLeft + cubeSize, y + cubeSize, planeNear - cubeSize,       0.0f, 0.0f, 1.0f, alpha,
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