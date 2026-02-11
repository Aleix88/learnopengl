#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <vector>
#include <glm/ext.hpp>

#include "Helpers/shader.hpp"

class Light {
public:
    unsigned int VBO, EBO, VAO = 0;
    Light();
    ~Light();

    void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
private:
    const Shader shader;
    std::vector<float> vertices;
    std::vector<unsigned int> elements;
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
    float cubeSize = 0.2f;

    std::vector<float> createCubeVertices();
    std::vector<unsigned int> createCubeElements();
    void draw();
    Shader createShader();
};

#endif