#include "food.h"

#include <iostream>
#include <print>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "constants.h"
#include "SceneObjects/cube.h"

Food::Food():shader(createShader()) {
    createVAO();
}

void Food::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    shader.use();
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setMat4("viewMatrix", viewMatrix);
    glBindVertexArray(VAO);

    if (glfwGetTime() - lastSpawnTime > 4.0f) {
        if (cube != nullptr) delete cube;
        cube = new Cube(glm::vec3(255/255.0f, 209.0f/255.0f, 102.0f/255.0f));

        foodCell = glm::vec2((int)(std::rand()%nColumns), (int)(rand()%nRows));
        glm::vec2 offset = glm::vec2(foodCell.x, -foodCell.y) * cubeSize;
        shader.setVec2("offset", offset);
        
        cube->bind(0);

        lastSpawnTime = glfwGetTime();
    }
    if (cube != nullptr) {
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

void Food::deleteFood() {
    if (cube != nullptr){
        delete cube;
        cube = nullptr;
        foodCell = glm::ivec2(-1);
    }
}

void Food::createVAO() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Position attribute
    glVertexAttribBinding(0,0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);

    // // Color attribute
    glVertexAttribBinding(1,0);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);
}

Shader Food::createShader() {
    std::string vertex = R"(
    #version 460 core

    layout (location = 0) in vec3 vPos;
    layout (location = 1) in vec4 vColor;

    out vec4 fColor;

    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;
    uniform vec2 offset;

    void main() {
        fColor = vColor;
        gl_Position = projectionMatrix * viewMatrix * vec4(vPos.x + offset.x, vPos.y, vPos.z + offset.y, 1.0f);
    }
    )";

    std::string fragment = R"(
    #version 460 core

    in vec4 fColor;
    out vec4 outColor;

    void main() {
        outColor = fColor;
    }
    )";

    return Shader(vertex, fragment, ShaderLoadingType::SOURCE_CODE);
}