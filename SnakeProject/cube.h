#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <vector>

class Cube {
public:
    GLuint VBO;
    GLuint EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> elements;

    Cube(float rColor, float gColor, float bColor);
    ~Cube();

    void draw();
    void bind(unsigned int index);

private:
    float r,g,b;
    std::vector<float> createCubeVertices();
    std::vector<unsigned int> createCubeElements();
};

#endif