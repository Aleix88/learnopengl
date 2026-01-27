#version 460 core

layout (location=0) in vec3 pos;
layout (location=1) in vec2 vTextureCoords;
out vec2 fTextureCoords;

uniform mat4 vTransMatrix;

void main() {
    fTextureCoords = vTextureCoords;
    gl_Position = vTransMatrix * vec4(pos.x, pos.y, pos.z, 1.0f);
}