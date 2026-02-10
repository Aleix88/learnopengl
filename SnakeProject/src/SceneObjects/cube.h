#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <vector>

class Cube {
public:
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    Cube(float rColor, float gColor, float bColor);
    ~Cube();

    void bind(unsigned int index);

private:
    std::vector<float> vertices;
    std::vector<unsigned int> elements;
    float r,g,b = 0.0f;

    std::vector<float> createCubeVertices();
    std::vector<unsigned int> createCubeElements();
    void draw();
};

#endif