#version 330 core

in vec3 myColor;
in vec2 TextCoord;
out vec4 color;

uniform sampler2D ourTexture;
uniform float mixValue;

void main() {
    color = texture(ourTexture, TextCoord);
}