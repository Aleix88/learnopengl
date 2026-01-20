#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textCoord;

out vec3 myColor;
out vec2 TextCoord;

uniform mat4 transMatrix;

void main() {
    gl_Position = transMatrix * vec4(pos.x, pos.y, pos.z, 1.0f);
    myColor = color;
    TextCoord = textCoord;
}