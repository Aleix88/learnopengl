#include "glad/glad.h"

#include "Helpers/shader.hpp"
#include "constants.h"

class Ground {
public:
    Ground(Shader* groundShader);
    ~Ground();

    void render();
private:
    Shader* shader;

    float vertices[7*4] = {
        // Position           // Color
        planeLeft, 0.0f, planeNear,     1.0f, 0.0f, 0.0f, 1.0f, // TOP-LEFT
        planeRight, 0.0f, planeNear,    1.0f, 0.0f, 0.0f, 1.0f, // TOP-RIGHT
        planeLeft, 0.0f, planeFar,      1.0f, 0.0f, 0.0f, 1.0f, // BOTTOM-LEFT
        planeRight, 0.0f, planeFar,     1.0f, 0.0f, 0.0f, 1.0f, // BOTTOM-RIGHT
    };

    unsigned int elements[3*2] = {
        0, 1, 2,
        1, 2, 3,
    };

    unsigned int VAO, VBO, EBO;

    void createVAO();
};