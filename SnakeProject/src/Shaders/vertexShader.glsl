#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vColor;

out vec4 fColor;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec2 posOffset;

void main() {
    fColor = vColor;
    gl_Position = projectionMatrix * viewMatrix * vec4(vPos.x + posOffset.x, vPos.y, vPos.z + posOffset.y, 1.0f);
}