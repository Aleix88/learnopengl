#ifndef CUBE_H
#define CUBE_H

#include <vector>

class Cube {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> elements;

    Cube();
    ~Cube();

    void draw();
    void bind(unsigned int index);

private:
    unsigned int VBO;
    unsigned int EBO;
    std::vector<float> createCubeVertices();
    std::vector<unsigned int> createCubeElements();
};

#endif