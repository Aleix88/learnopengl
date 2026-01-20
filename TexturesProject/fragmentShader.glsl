#version 330 core

in vec3 myColor;
in vec2 TextCoord;
out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D anotherTexture;
uniform float mixValue;

void main() {
    color = mix(texture(ourTexture, TextCoord), texture(anotherTexture, TextCoord), mixValue);
    // color = texture(ourTexture, TextCoord) /** vec4(myColor, 1.0)*/;
}