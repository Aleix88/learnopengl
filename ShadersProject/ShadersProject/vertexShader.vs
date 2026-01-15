#version 330 core
in vec3 aPos;
in vec4 myColor;
out vec4 aColor;

void main() {
   aColor = myColor;
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
