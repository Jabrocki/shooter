#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const std::string& vertexpath, const std::string& fragmentpath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Set uniforms
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    void SetUnifrom1i(const std::string& name, int i0);
    void SetUniformMat4f(const std::string& name, const glm::mat4& mat4f);
private:
    unsigned int m_RendererID;
    std::string m_VertexPath;
    std::string m_FragmentPath;
    std::unordered_map<std::string, int> m_UniformLocationCache;

    int GetUniformLocation(const std::string& name);
    std::string ParseShaderCode(const std::string& path);
};

