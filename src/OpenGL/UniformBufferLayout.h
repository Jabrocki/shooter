#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

struct UniformBufferElement {
  GLenum glType;
  std::string name;

  unsigned int offset;
  unsigned int size;
};

class UniformBufferLayout {
public:
  UniformBufferLayout() : m_BlockSize(0) {};
  ~UniformBufferLayout() {};

  template <typename T> void Push(const std::string &name) {
    static_assert(false);
  }

  // ---- Specializations: scalars ----
  template <> void Push<float>(const std::string &name) {
    AddScalar(GL_FLOAT, name);
  }
  template <> void Push<int>(const std::string &name) {
    AddScalar(GL_INT, name);
  }
  template <> void Push<unsigned int>(const std::string &name) {
    AddScalar(GL_UNSIGNED_INT, name);
  }
  template <> void Push<bool>(const std::string &name) {
    AddScalar(GL_BOOL, name);
  }

  // ---- Specializations: vectors (GLM) ----
  template <> void Push<glm::vec2>(const std::string &name) {
    AddVector(GL_FLOAT_VEC2, 2, name);
  }
  template <> void Push<glm::vec3>(const std::string &name) {
    AddVector(GL_FLOAT_VEC3, 3, name);
  }
  template <> void Push<glm::vec4>(const std::string &name) {
    AddVector(GL_FLOAT_VEC4, 4, name);
  }

  // ---- Specializations: matrices (column-major, std140) ----
  template <> void Push<glm::mat3>(const std::string &name) {
    AddMatrix(GL_FLOAT_MAT3, 3, 3, name);
  }
  template <> void Push<glm::mat4>(const std::string &name) {
    AddMatrix(GL_FLOAT_MAT4, 4, 4, name);
  }

  const std::vector<UniformBufferElement> &GetElements() const {
    return m_Elements;
  }
  uint32_t GetBlockSize() const { return m_BlockSize; }

private:
  std::vector<UniformBufferElement> m_Elements;
  unsigned int m_BlockSize;

  static unsigned int RoundUp(unsigned int value, unsigned int alignment) {
    return (value + (alignment - 1)) & ~(alignment - 1);
  }
  // ---- std140 base alignments & sizes for single elements ----
  static uint32_t BaseAlignScalar() { return 4; }
  static uint32_t SizeScalar() { return 4; }

  static uint32_t BaseAlignVector(int comps) { return comps == 2 ? 8u : 16u; }
  static uint32_t SizeVector(int comps) {
    return comps == 2 ? 8u : 16u;
  } // vec3 occupies 16

  static uint32_t BaseAlignMatrix() { return 16u; }
  static uint32_t SizeMatrix(int cols) {
    return static_cast<uint32_t>(cols) * 16u;
  }

  void AddScalar(GLenum glType, const std::string &name) {
    const uint32_t A = BaseAlignScalar();
    const uint32_t S = SizeScalar();

    UniformBufferElement e{};
    e.glType = glType;
    e.name = name;
    e.offset = RoundUp(m_BlockSize, A);
    e.size = S;

    m_BlockSize = e.offset + e.size;
    m_Elements.push_back(e);
  }

  void AddVector(GLenum glType, int comps, const std::string &name) {
    const uint32_t A = BaseAlignVector(comps);
    const uint32_t S = SizeVector(comps);

    UniformBufferElement e{};
    e.glType = glType;
    e.name = name;
    e.offset = RoundUp(m_BlockSize, A);
    e.size = S;

    m_BlockSize = e.offset + e.size;
    m_Elements.push_back(e);
  }

  void AddMatrix(GLenum glType, int cols, int rows, const std::string &name) {
    const uint32_t A = BaseAlignMatrix();
    const uint32_t S = SizeMatrix(cols);

    UniformBufferElement e{};
    e.glType = glType;
    e.name = name;
    e.offset = RoundUp(m_BlockSize, A);
    e.size = S;

    m_BlockSize = e.offset + e.size;
    m_Elements.push_back(e);
  }
};
