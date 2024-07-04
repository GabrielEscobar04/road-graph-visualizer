#ifndef SHADER_H
#define SHADER_H

#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h> 
#include <glm/glm.hpp>

class Shader {
public:
    Shader(std::string vertexPath, std::string fragmentPath);
    ~Shader();

    void use();
    void setMat4(const std::string& name, glm::mat4 mat);
    void setVec3(const std::string& name, glm::vec3 vec);
    void setBool(const std::string& name, bool value);
    GLuint getAttributeLocation(const std::string& name) const;

private:
    GLuint Program;

private:
    void compile(const char* vertexCode, const char* fragmentCode);
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif
