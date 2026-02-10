#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <vector>
#include <glm/ext.hpp>

class Cube {
public:
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    Cube(glm::vec3 color);
    ~Cube();

    void bind(unsigned int index);

private:
    std::vector<float> vertices;
    std::vector<unsigned int> elements;
    glm::vec3 color;

    std::vector<float> createCubeVertices();
    std::vector<unsigned int> createCubeElements();
    void draw();
};

#endif