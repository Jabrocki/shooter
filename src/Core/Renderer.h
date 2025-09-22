#pragma once

// STL
#include <unordered_map>

// GLM
#include <glm/glm.hpp>

// Scene
#include "Scene.h"

// OpenGL
#include "../OpenGL/FrameBuffer.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/UniformBuffer.h"

class Renderer {
public:
  Renderer();
  // TODO: implement deconstructor
  void AddShader(std::string &objType, Shader *);
  void Draw(Scene &scene, const glm::mat4 &viewMatrix,
            const glm::mat4 &projectionMatrix, const glm::vec3 &cameraPosition);

private:
  UniformBuffer mUBO;
  FrameBuffer mFBO;
  Shader mScreenShader = Shader("res/shaders/ppShader/ppShader.vert",
                                "res/shaders/ppShader/ppShader.frag");
  std::unordered_map<std::string, Shader *> mShaderRegistry;

  void DrawOpaque(Scene &scene);
  void DrawTransparent(Scene &scene, const glm::vec3 &cameraPosition);
  void Clear();

  unsigned int quadVAO, quadVBO;
};
