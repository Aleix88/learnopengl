#include<glm/glm.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    TURBO
};

class Camera {
public:
    glm::vec3 position, direction;
    float speed, sensitivity, turbo;

    Camera(
        glm::vec3 cameraPosition, 
        glm::vec3 cameraDirection = glm::vec3(0,0,-1),
        float cameraSpeed = 3.0f,
        float cameraSensitivity = 0.1f,
        float cameraTurbo = 1.0f
    ) {
        position = cameraPosition;
        direction = cameraDirection;
        speed = cameraSpeed;
        sensitivity = cameraSensitivity;
        turbo = cameraTurbo;
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
        float yawRad = glm::radians(yaw);
        float pitchRad = glm::radians(pitch);
        direction = glm::vec3
        (
            sin(yawRad) * cos(pitchRad),
            sin(pitchRad),
            -cos(yawRad) * cos(pitchRad)
        );
        direction = glm::normalize(direction);
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

    glm::mat4 viewMatrix() {
        return glm::lookAt(position, direction + position, up);
    }

private:
    float pitch = 0.0f;
    float yaw = 0.0f;
    float lastMouseX = -1.0f;
    float lastMouseY = -1.0f;
    bool isTurboActive = false;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};