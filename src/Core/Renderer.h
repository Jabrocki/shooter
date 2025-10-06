#pragma once

// STL
#include <unordered_map>

// GLM
#include <glm/glm.hpp>

// Scene
#include "Scene.h"

// OpenGL
#include "../OpenGL/CubeMap.h"
#include "../OpenGL/FrameBuffer.h"
#include "../OpenGL/ScreenQuad.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/UniformBuffer.h"

// Pass through shader code
extern const char *vertPassThrough;
extern const char *fragPassThrought;

class Renderer {
public:
  Renderer();
  // TODO: implement deconstructor
  void AddRegularShader(std::string &objType, Shader *);
  void AddPostProcessingShader(Shader *shader);
  void Draw(Scene &scene, const glm::mat4 &viewMatrix,
            const glm::mat4 &projectionMatrix, const glm::vec3 &cameraPosition);

  FrameBuffer FBO;
  FrameBuffer pingPongFBO[2];

  // CubeMaps
  void AddCubeMap(const std::string &name, CubeMap *cubeMap);
  void RemoveCubeMap(const std::string &name);
  void SetActiveCubeMap(const std::string &name);

private:
  std::unique_ptr<Shader> mShaderPassThrough = std::make_unique<Shader>(
      Shader::fromSource(vertPassThrough, fragPassThrought));

  ScreenQuad mScreenQuad;
  UniformBuffer mUBO;

  std::unordered_map<std::string, Shader *> mRegularShaderRegistry;
  std::vector<Shader *> mPostProcesingShaders;

  void DrawOpaque(Scene &scene);
  void DrawTransparent(Scene &scene, const glm::vec3 &cameraPosition);
  void DrawCubeMap(const glm::mat4 &viewMatirx, const glm::mat4 &projMAtrix);

  void Clear();

  // CubeMaps
  std::unordered_map<std::string, CubeMap *> mCubeMapsRegistry;
  std::string mActiveCubeMap;
};
