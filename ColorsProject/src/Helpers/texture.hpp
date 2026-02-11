#include <string>
#include <glad/glad.h>
#include <print>

#include "libs/stb_image.h"

class Texture {
public:
    unsigned int ID;
    unsigned int BIND_INDEX;

    Texture(
        std::string source, 
        int bindIndex,
        int wrapT = GL_REPEAT, 
        int wrapS = GL_REPEAT, 
        int magFilter = GL_LINEAR, 
        int minFilter = GL_LINEAR_MIPMAP_LINEAR
    ) {
        BIND_INDEX = bindIndex;
        glGenTextures(1, &ID);
        glActiveTexture(GL_TEXTURE0 + bindIndex);
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

        // OPENGL stores textures in memory bottom up, the first byte of the image is the top-left
        // But the UV coordinates reads the image starting at bottom-left. Thats why the image must 
        // be flipped when loading
        // https://www.puredevsoftware.com/blog/2018/03/17/texture-coordinates-d3d-vs-opengl/
        bool isPNG = source.find(".png") != std::string::npos;
        GLenum imageType = isPNG ? GL_RGBA : GL_RGB;
        int width, height, nChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(source.c_str(), &width, &height, &nChannels, 0);
        if (data == NULL) {
            std::println("Error loading image");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageType, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
};