#pragma once
#include <glad/glad.h>
#include <vector>

struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  bool normalized;

  static unsigned int GetSizeOfType(unsigned int type) {
    switch (type) {
    case GL_FLOAT:
      return 4;
    case GL_UNSIGNED_INT:
      return 4;
    case GL_UNSIGNED_BYTE:
      return 1;
    case GL_INT:
      return 4;
    case GL_DOUBLE:
      return 8;
    default:
      return 0;
    }
    return 0;
  }
};

class VertexBufferLayout {
private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;

public:
  VertexBufferLayout() : m_Stride(0) {}

  template <typename T> void Push(unsigned int count) { static_assert(false); }

  template <> void Push<float>(unsigned int count) {
    m_Elements.push_back(VertexBufferElement{GL_FLOAT, count, false});
    m_Stride += sizeof(GLfloat) * count;
  }

  template <> void Push<unsigned int>(unsigned int count) {
    m_Elements.push_back(VertexBufferElement{GL_UNSIGNED_INT, count, false});
    m_Stride += sizeof(GLuint) * count;
  }

  template <> void Push<unsigned char>(unsigned int count) {
    m_Elements.push_back(VertexBufferElement{GL_UNSIGNED_BYTE, count, true});
    m_Stride += sizeof(GLubyte) * count;
  }

  template <> void Push<int>(unsigned int count) {
    m_Elements.push_back(VertexBufferElement{GL_INT, count, false});
    m_Stride += sizeof(GLint) * count;
  }

  inline std::vector<VertexBufferElement> GetElements() const & {
    return m_Elements;
  }
  inline unsigned int GetStride() const { return m_Stride; }
};
