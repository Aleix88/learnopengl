#include "ground.h"

#include <string>

Ground::Ground():shader(createShader()) {
    createVAO();
}

Ground::~Ground() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Ground::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    shader.use();
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setMat4("viewMatrix", viewMatrix);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_INT, 0);
};

void Ground::createVAO() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexBuffer(0, VBO, 0, sizeof(float) * 7);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribBinding(1, 0);
    glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexAttribArray(1);   
}

Shader Ground::createShader() {
    std::string vertex = R"(
    #version 460 core

    layout (location = 0) in vec3 vPos;
    layout (location = 1) in vec4 vColor;

    out vec4 fColor;

    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;

    void main() {
        fColor = vColor;
        gl_Position = projectionMatrix * viewMatrix * vec4(vPos, 1.0f);
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