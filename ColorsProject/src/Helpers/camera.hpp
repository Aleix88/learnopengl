#include<glm/glm.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    TURBO
};

unsigned char CAMERA_POS_DIR_LOGS = 1 << 1;
unsigned char CAMERA_ANGLE_LOGS = 1 << 2;

class Camera {
public: 
    glm::vec3 position, direction;
    float speed, sensitivity, turbo;
    float pitch, yaw = 0.0f;

    Camera(
        glm::vec3 cameraPosition, 
        float cameraPitch = 0.0f,
        float cameraYaw = 0.0f,
        float cameraSpeed = 3.0f,
        float cameraSensitivity = 0.1f,
        float cameraTurbo = 1.0f
    ) {
        position = cameraPosition;
        pitch = cameraPitch;
        yaw = cameraYaw;
        speed = cameraSpeed;
        sensitivity = cameraSensitivity;
        turbo = cameraTurbo;
        calculateDirection();
    }

    void processMouseInput(double xpos, double ypos) {
        float deltaX = xpos - lastMouseX;
        float deltaY = ypos - lastMouseY;
        if (lastMouseX != -1.0f && lastMouseY != -1.0f) {
            pitch += -sensitivity * deltaY;
            float pitchLimit = 80.0f;
            if (pitch > pitchLimit) {
                pitch = pitchLimit;
            } else if (pitch < -pitchLimit) {
                pitch = -pitchLimit;
            }
            yaw += sensitivity * deltaX;
        }

        calculateDirection();
        lastMouseX = xpos;
        lastMouseY = ypos;
    }

    void processKeyboardInput(CameraMovement movement, float deltaTime) {
        float cameraTurbo = turbo * isTurboActive;
        float cameraSpeed = ((cameraTurbo * speed) + speed) * deltaTime;
        if (movement == ::FORWARD)
            position += cameraSpeed * direction;
        if(movement == ::BACKWARD)
            position -= cameraSpeed * direction;
        if(movement == ::LEFT)
            position -= cameraSpeed * glm::normalize(glm::cross(direction, up));
        if(movement == ::RIGHT)
            position += cameraSpeed * glm::normalize(glm::cross(direction, up));
    }

    void setTurbo(bool active) {
        isTurboActive = active;
    }

    void setConfig(unsigned char mask) {
        configMask = mask;
    }

    glm::mat4 viewMatrix() {
        if (isConfigEnabled(CAMERA_POS_DIR_LOGS)) {
            printVector("Position", position);
            printVector("Direction", direction);
        }
        return glm::lookAt(position, direction + position, up);
    }

private:
    float lastMouseX = -1.0f;
    float lastMouseY = -1.0f;
    bool isTurboActive = false;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    unsigned char configMask = 0;

    bool isConfigEnabled(unsigned char mask) {
        return configMask & mask;
    }

    void printVector(const char* name, glm::vec3 v) {
        std::println("{0}: {1}, {2}, {3}", name, v.x, v.y, v.z);
    }

    void calculateDirection() {
        if (isConfigEnabled(CAMERA_ANGLE_LOGS)) {
            std::println("Yaw: {0}, Pitch: {1}", yaw, pitch);
        }
        float yawRad = glm::radians(yaw);
        float pitchRad = glm::radians(pitch);
        direction = glm::vec3
        (
            sin(yawRad) * cos(pitchRad),
            sin(pitchRad),
            -cos(yawRad) * cos(pitchRad)
        );
        direction = glm::normalize(direction);
    }
};

void cameraWASD(GLFWwindow* window, Camera& camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W)) 
        camera.processKeyboardInput(::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A)) 
        camera.processKeyboardInput(::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S)) 
        camera.processKeyboardInput(::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D)) 
        camera.processKeyboardInput(::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        camera.setTurbo(true);
    else 
        camera.setTurbo(false);
}