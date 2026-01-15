#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void configureGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

GLFWwindow* createWindow(int height, int width) {
    GLFWwindow* window = glfwCreateWindow(height, width, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::runtime_error("Failed to initialize GLAD");
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void loadOpenGLPointers() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::runtime_error("Failed to initialize GLAD");
    }
}

const char *vertexShaderSource = "#version 330 core\n"
    "in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 myColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = myColor;\n"
    "}\n\0";

void run(GLFWwindow* window);

int main(int argc, const char * argv[]) {
    configureGLFW();
    try {
        GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT);
        loadOpenGLPointers();
        run(window);
    } catch(const std::exception& e) {
        glfwTerminate();
        return -1;
    }

    return EXIT_SUCCESS;
}

void run(GLFWwindow* window) {
    // -- Vertex shader --
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compile error: " << infoLog;
    }

    // -- Fragment shader --
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compile error: " << infoLog;
    }

    // -- Shader program --
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program link error\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // -- Configure shape --
    float vertexs[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

    // Two options declar the location of the attribute in the shader using `layout(location = 0) in...`
    // or use glGetAttribLocation to find the location of the attribute (avoiding the layout(locaition = 0))
    int attributeLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(attributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(attributeLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.1f,0.2f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);

        float time = glfwGetTime();
        float rColor = abs(sin(time/2));
        float gColor = abs(sin(time/2 +1.5f));
        std::cout << "R: " << rColor << ", G: " << gColor << " - Time: " << time << std::endl;
        int colorAttribute = glGetUniformLocation(shaderProgram, "myColor");
        glUniform4f(colorAttribute, rColor, gColor, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}
