#pragma once
#include <GLAD/glad.h>
#include <string>
#include <glm/glm.hpp>

namespace ngiv
{




class GLSLProgram
{
public:
    GLSLProgram();
    ~GLSLProgram();

    void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath = "");

    void compileShadersfromsource(const char* vertexsource, const char* fragmentsource);

    void linkShaders();

    void addAttribute(const std::string& attributeName);

    GLint getUniformLocation(const std::string& uniformName);

    void setVec4(std::string name,const glm::vec4& val)
    {
        glUniform4fv(getUniformLocation(name), 1, &val[0]);
    }

    void setVec3(std::string name, glm::vec3 val)
    {
        glUniform3fv(getUniformLocation(name), 1, &val[0]);
    }

    void setVec2(std::string name, glm::vec2 val)
    {
        glUniform2fv(getUniformLocation(name), 1, &val[0]);
    }

    void setFloat(std::string name, float val)
    {
        glUniform1f(getUniformLocation(name), val);
    }
    void setInt(std::string name, int val)
    {
        glUniform1i(getUniformLocation(name), val);
    }


    void use();
    void unuse();

    void dispose();
private:

    int _numAttributes;

    void compileShader(const char* source, std::string name, GLuint id);

    GLuint _programID;

    GLuint _vertexShaderID;
    GLuint _fragmentShaderID;
    GLuint _geometryShaderID = 0;
};

}
