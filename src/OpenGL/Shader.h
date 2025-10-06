#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader {
public:
  static Shader fromFile(const std::string &vertexPath,
                         const std::string &fragmentPath);
  static Shader fromSource(const std::string &vertexSource,
                           const std::string &fragmentSource);

  ~Shader();

  void Bind() const;
  void Unbind() const;

  // Set uniforms
  void SetUniform4f(const std::string &name, float f0, float f1, float f2,
                    float f3);
  void SetUniform1i(const std::string &name, int i0);
  void SetUniform1f(const std::string &name, float f0);

  void SetUniformMat4f(const std::string &name, const glm::mat4 &mat4f);
  void SetUniformVec3f(const std::string &name, const glm::vec3 &vec3f);
  void SetUniformVec4f(const std::string &name, const glm::vec4 &vec4f);

private:
  unsigned int mRendererID;
  std::unordered_map<std::string, int> m_UniformLocationCache;

  int GetUniformLocation(const std::string &name);
  std::string ParseShaderCode(const std::string &path);
};
