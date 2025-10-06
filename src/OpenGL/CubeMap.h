#pragma once

// STL
#include <string>

// OpenGL
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

extern float skyboxVertices[];
extern unsigned int skyboxIndices[];
extern std::string cubeMapShaderSourceVert;
extern std::string cubeMapShaderSourceFrag;

class CubeMap {
public:
  CubeMap(const std::string &rightPath, const std::string &leftPath,
          const std::string &topPath, const std::string &bottomPath,
          const std::string &frontPath, const std::string &backPath);

  void Draw(glm::mat4 view, glm::mat4 projection);

private:
  unsigned int mRendererId;
  VertexArray mVAO;
  VertexBuffer mVBO;
  VertexBufferLayout mLayout;
  IndexBuffer mIBO;
  Shader mShader;
};
