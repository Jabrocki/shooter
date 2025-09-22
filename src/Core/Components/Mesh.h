#pragma once

#include <glm/glm.hpp>

#include <vector>

// OpenGL
#include "../../OpenGL/IndexBuffer.h"
#include "../../OpenGL/Shader.h"
#include "../../OpenGL/Texture.h"
#include "../../OpenGL/VertexArray.h"
#include "../../OpenGL/VertexBuffer.h"
// Example Vertex layout
struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
};

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
       std::vector<Texture> textures);

  void Draw(Shader &shader, const glm::mat4 &model);

private:
  VertexArray m_VAO;
  IndexBuffer m_IBO;
  VertexBuffer m_VBO;
};
