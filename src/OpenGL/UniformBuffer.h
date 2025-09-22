#pragma once

#include "UniformBufferLayout.h"
#include <string>
#include <unordered_map>

struct View {
  uint32_t offset = 0;
  uint32_t size = 0;
  GLenum glType = 0;
};

class UniformBuffer {
public:
  UniformBuffer(const UniformBufferLayout &layout);
  ~UniformBuffer();

  void Bind();

  // Add setters if needed
  void SetUniformMat4f(const std::string &name, const glm::mat4 &mat4f) {
    SetRaw(name, &mat4f, sizeof(glm::mat4));
  }
  void SetUniformVec3f(const std::string &name, const glm::vec3 &vec3f) {
    SetRaw(name, &vec3f, sizeof(glm::vec3));
  }

private:
  unsigned int m_RendererID;
  unsigned int m_SizeBytes;
  const UniformBufferLayout &m_Layout;
  std::unordered_map<std::string, View> m_Views;

  void SetRaw(const std::string &name, const void *src, size_t sizeBytes);
  void IndexElements();
};
