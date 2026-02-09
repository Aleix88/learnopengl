#include "snake.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <print>

#include "constants.h"

Snake::Snake(Shader* snakeShader) {
    createVAO();
    directions = {
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
    };
    nextDirection = directions[0];
    positions = {
        glm::vec3(2, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 0, 0),
    };
    cubes = {
        new Cube(0.0f, 1.0f, 0.0f),
        new Cube(0.0f, 1.0f, 0.0f),
        new Cube(0.0f, 1.0f, 0.0f),
    };
    shader = snakeShader;
    std::println("CREATING SNAKE INSTANCE {0}", VAO);
}

Snake::~Snake() {
    std::println("DESTROY SNAKE INSTANCE");
}

void Snake::createVAO() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // Position attribute
    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribBinding(1, 0);
    glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);
}

void Snake::render(int keyPressed) {
    glBindVertexArray(VAO);
    calculateNextDirection(keyPressed);
    move();
}

void Snake::calculateNextDirection(int keyPressed) {   
    if (keyPressed == GLFW_KEY_W) {
        nextDirection = glm::vec3(0,0,1);
    }
    if (keyPressed == GLFW_KEY_S) {
        nextDirection = glm::vec3(0,0,-1);
    }
    if (keyPressed == GLFW_KEY_A) {
        nextDirection = glm::vec3(-1,0,0);
    }
    if (keyPressed == GLFW_KEY_D) {
        nextDirection = glm::vec3(1,0,0);
    }

    if (nextDirection != -directions[0]) {
        directions[0] = nextDirection;
    }
}

void Snake::move() {
    if (glfwGetTime() - lastMoveTimestamp > 0.1f) {
        glm::ivec3 newHeadCellPos = positions[0] + directions[0];
        // std::println("X: {0}, Y: {1}", newHeadCellPos.x, newHeadCellPos.z);
        std::println("{0}", cubeSize);
        if (newHeadCellPos.x > 19) {
            newHeadCellPos.x = 0;
        } else if (newHeadCellPos.x < 0) {
            newHeadCellPos.x = 19;
        } else if (newHeadCellPos.z > 19) {
            newHeadCellPos.z = 0;
        } else if (newHeadCellPos.z < 0) {
            newHeadCellPos.z = 19;
        }

        for (int i = positions.size() - 1; i >= 0; i--) {
            if (i == 0) {
                positions[i] = newHeadCellPos;
            } else {
                directions[i] = directions[i-1];
                positions[i] = positions[i-1];
                if (positions[i] == newHeadCellPos) {
                    std::println("You losed you dum dum...");
                    // glfwSetWindowShouldClose(window, true);
                    break;
                }
            }
            lastMoveTimestamp = glfwGetTime();
        }
    }

    for (int i = 0; i < positions.size(); i++) {
        glm::vec3 offset = glm::vec3(
            positions[i].x * cubeSize, 
            0, 
            -positions[i].z * cubeSize
        );
        
        shader->setVec2("posOffset", glm::vec2(offset.x, offset.z));
        (cubes[i])->bind(0);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

// if (keyPressed == GLFW_KEY_SPACE) {
//     cubes.push_back(new Cube(0,1.0,0));
//     (*(cubes.end() - 1))->draw();
//     glm::vec3 prevPosition = *(snakePositions.end() - 1);
//     glm::vec3 prevDirection = *(snakeDirections.end() - 1);
//     glm::vec3 newElementPos = prevPosition - cubeSize * prevDirection;
//     snakePositions.push_back(newElementPos);
//     snakeDirections.push_back(prevDirection);
// }