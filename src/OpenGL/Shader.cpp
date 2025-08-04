#include "Shader.h"

#include "Debug.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexpath, const std::string& fragmentPath)
    : m_VertexPath(vertexpath), m_FragmentPath(fragmentPath)
{
    std::string vShaderCode = ParseShaderCode(m_VertexPath);
    std::string fShaderCode = ParseShaderCode(m_FragmentPath);

    const char* vShaderSource = vShaderCode.c_str();
    const char* fShaderSource = fShaderCode.c_str();

    unsigned int vertex, fragment;

    // Compile vertex shader
    GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertex, 1, &vShaderSource, NULL));
    GLCall(glCompileShader(vertex));

    // Compile fragment shader
    GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragment, 1, &fShaderSource, NULL));
    GLCall(glCompileShader(fragment));

    // Link shader program
    GLCall(m_RendererID = glCreateProgram());
    GLCall(glAttachShader(m_RendererID, vertex));
    GLCall(glAttachShader(m_RendererID, fragment));
    GLCall(glLinkProgram(m_RendererID));

    GLCall(glDeleteShader(vertex));
    GLCall(glDeleteShader(fragment));
}

std::string Shader::ParseShaderCode(const std::string& path)
{
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;

        shaderStream << shaderFile.rdbuf();

        shaderFile.close();

        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: "<< path << std::endl;
    }

    return shaderCode;
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform4f(location, f0, f1, f2, f3));
}

void Shader::SetUnifrom1i(const std::string& name, int i0)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform1i(location, i0));
}

void Shader::SetUniformMat4f(const std::string& name,const glm::mat4& mat4f) 
{
    int location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &mat4f[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) 
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) 
        std::cout << "Warning: uniform '" << name << "' doesn't exists!" << std::endl;
        
    m_UniformLocationCache[name] = location;
    return location;
}
