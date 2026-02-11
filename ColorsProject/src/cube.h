#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <vector>
#include <glm/ext.hpp>

#include "Helpers/shader.hpp"

class Cube {
public:
    unsigned int VBO, EBO, VAO = 0;
    Cube(glm::vec3 color, float size);
    ~Cube();

    void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
private:
    const Shader shader;
    std::vector<float> vertices;
    std::vector<unsigned int> elements;
    glm::vec3 color;
    float cubeSize = 0.4f;

    std::vector<float> createCubeVertices();
    std::vector<unsigned int> createCubeElements();
    void draw();
    Shader createShader();
};

#endif