#include <glm/ext.hpp>

#include "glad/glad.h"
#include "Helpers/shader.hpp"
#include "constants.h"

class Ground {
public:
    Ground();
    ~Ground();

    void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
private:
    const Shader shader;

    float vertices[7*4] = {
        // Position           // Color
        0,                   0.0f, 0,                     1.0f, 0.0f, 0.0f, 1.0f, // BOTTOM-LEFT
        nColumns * cubeSize, 0.0f, 0,                     1.0f, 0.0f, 0.0f, 1.0f, // BOTTOM-RIGHT
        0,                   0.0f, -nRows * cubeSize,     1.0f, 0.0f, 0.0f, 1.0f, // TOP-LEFT
        nColumns * cubeSize, 0.0f, -nRows * cubeSize,     1.0f, 0.0f, 0.0f, 1.0f, // TOP-RIGHT
    };

    unsigned int elements[3*2] = {
        0, 1, 2,
        1, 2, 3,
    };

    unsigned int VAO, VBO, EBO;

    void createVAO();
    Shader createShader();
};