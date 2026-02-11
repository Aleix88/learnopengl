#ifndef SHADER_H
#define SHADER_H

#include <glm/ext.hpp>
#include <glad/glad.h>
#include<glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum ShaderLoadingType {
    FILE_PATH,
    SOURCE_CODE
};

class Shader {
public:
    unsigned int ID;
    
    const std::string FRAGMENT = "FRAGMENT";
    const std::string VERTEX = "VERTEX";
    const std::string PROGRAM = "PROGRAM";

    Shader(std::string vertexValue, std::string fragmentValue, ShaderLoadingType type) {
        std::string vShaderCode;
        std::string fShaderCode;
        if (type == ::FILE_PATH) {
            std::string vShaderFileContent = readFile(vertexValue.c_str());
            std::string fShaderFileContent = readFile(fragmentValue.c_str());
            vShaderCode = vShaderFileContent;
            fShaderCode = fShaderFileContent;
        } else if (type == ::SOURCE_CODE) {
            vShaderCode = vertexValue;
            fShaderCode = fragmentValue;
        }

        const char* vCode = vShaderCode.c_str();
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vCode, NULL);
        glCompileShader(vertex);
        checkErrors(vertex, VERTEX);

        const char* fCode = fShaderCode.c_str();
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fCode, NULL);
        glCompileShader(fragment);
        checkErrors(fragment, FRAGMENT);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkErrors(ID, PROGRAM);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() const {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setMat4(const std::string &name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void setVec2(const std::string &name, glm::vec2 value) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
    }

    void setVec3(const std::string &name, glm::vec3 value) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
    }

private:
    std::string readFile(const char* path) const {
        std::string text;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            text = stream.str();
        } catch(std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        return text;
    }

    void checkErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != PROGRAM) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

#endif
