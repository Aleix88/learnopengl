#ifndef FOOD_H
#define FOOD_H

#include <glm/ext.hpp>

#include "Helpers/shader.hpp"
#include "SceneObjects/cube.h"

class Food {
public:
    Food();
    void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

private:
    unsigned int VAO;
    const Shader shader;
    float lastSpawnTime;

    Cube* cube;

    void createVAO();
    Shader createShader();
};

#endif