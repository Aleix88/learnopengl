#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <glm/ext.hpp>

#include "SceneObjects/cube.h"
#include "Helpers/shader.hpp"

class Snake {
public:
    bool collisionDetected = false;
    bool snakeDied = false;

    Snake();
    ~Snake();

    void createVAO();
    void render(
        int keyPressed, 
        glm::mat4 projectionMatrix, 
        glm::mat4 viewMatrix,
        glm::ivec2 foodCell
    );

private:
    const Shader shader;
    std::vector<glm::ivec3> directions;
    std::vector<glm::ivec3> positions;
    std::vector<Cube*> cubes;
    glm::ivec3 nextDirection;
    float lastMoveTimestamp = 0.0;
    unsigned int VAO = 0;
    glm::vec3 color = glm::vec3(239.0f/255.0f, 71.0f/255.0f, 111.0f/255.0f);

    void calculateNextDirection(int keyPressed);
    void move();
    void checkFoodCollision(glm::ivec2);
    Shader createShader();
};


#endif