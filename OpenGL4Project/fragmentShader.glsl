#version 460 core

in vec2 fTextureCoords;
in mat4 fTransMatrix;

out vec4 FragColor;

uniform sampler2D boxTexture;
uniform sampler2D faceTexture;

void main() {
    FragColor = mix(
        texture(boxTexture, fTextureCoords), 
        texture(faceTexture, fTextureCoords), 
        0.5
    );
}