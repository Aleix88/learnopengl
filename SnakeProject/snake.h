#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <glm/ext.hpp>

#include "cube.h"
#include "Helpers/shader.hpp"

class Snake {
public:
    std::vector<glm::ivec3> directions;
    std::vector<glm::ivec3> positions;
    std::vector<Cube*> cubes;
    glm::ivec3 nextDirection;
    float lastMoveTimestamp = 0.0;
    unsigned int VAO;
    Shader* shader;

    Snake(Shader* snakeShader);
    ~Snake();

    void createVAO();
    void render(int keyPressed);

private:
    void calculateNextDirection(int keyPressed);
    void move();
};

// if (keyPressed == GLFW_KEY_SPACE) {
//     cubes.push_back(new Cube(0,1.0,0));
//     (*(cubes.end() - 1))->draw();
//     glm::vec3 prevPosition = *(snakePositions.end() - 1);
//     glm::vec3 prevDirection = *(snakeDirections.end() - 1);
//     glm::vec3 newElementPos = prevPosition - cubeSize * prevDirection;
//     snakePositions.push_back(newElementPos);
//     snakeDirections.push_back(prevDirection);
// }

#endif