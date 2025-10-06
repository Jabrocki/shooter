#include "Renderer.h"

// GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

// OpenGL
#include "../OpenGL/Debug.h"
#include "../OpenGL/UniformBufferLayout.h"

// STL
#include <map>
#include <string>

// Future: move to the settings file
#define MAX_LIGHTS 5

const char *vertPassThrough = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;
out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char *fragPassThrought = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D uScreenTexture;

void main()
{
    FragColor = texture(uScreenTexture, TexCoords);
}
)";

Renderer::Renderer()
    : mUBO([&] {
        UniformBufferLayout uboLayout;
        uboLayout.Push<glm::mat4>("uView");
        uboLayout.Push<glm::mat4>("uProjection");
        uboLayout.Push<glm::vec3>("uViewPosition");
        return UniformBuffer(uboLayout);
      }()),
      FBO(800.0f, 600.0f), // TODO: dynamic screen size
      pingPongFBO{FrameBuffer(800.0f, 600.0f), FrameBuffer(800.0f, 600.0f)} {

  // Depth test
  GLCall(glEnable(GL_DEPTH_TEST));

  // Blending
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Face culling
  GLCall(glEnable(GL_CULL_FACE));
  GLCall(glCullFace(GL_BACK));
  GLCall(glFrontFace(GL_CCW));

  // Binding UBO
  mUBO.Bind();
}

void Renderer::AddRegularShader(std::string &objType, Shader *shader) {
  mRegularShaderRegistry[objType] = shader;
}

void Renderer::AddPostProcessingShader(Shader *shader) {
  mPostProcesingShaders.push_back(shader);
}

void Renderer::Draw(Scene &scene, const glm::mat4 &viewMatrix,
                    const glm::mat4 &projMatrix,
                    const glm::vec3 &cameraPostion) {
  // Draw into framebuffer
  FBO.Bind();
  // Set global uniforms
  mUBO.SetUniformMat4f("uView", viewMatrix);
  mUBO.SetUniformMat4f("uProjection", projMatrix);
  mUBO.SetUniformVec3f("uViewPosition", cameraPostion);

  DrawCubeMap(viewMatrix, projMatrix);
  DrawOpaque(scene);
  DrawTransparent(scene, cameraPostion);

  FBO.UnBind();
  // Post processing
  unsigned int readTex = FBO.GetTextureID();

  if (mPostProcesingShaders.empty()) {
    mShaderPassThrough->Bind();
    mScreenQuad.Draw(readTex);
    return;
  }

  unsigned int writeFBO = 0;
  unsigned int readFBO = 1;

  for (unsigned int i = 0; i < mPostProcesingShaders.size(); i++) {

    pingPongFBO[writeFBO].Bind();
    unsigned int writeTex = pingPongFBO[writeFBO].GetTextureID();

    mPostProcesingShaders[i]->Bind();
    mScreenQuad.Draw(readTex);

    pingPongFBO[writeFBO].UnBind();
    readTex = writeTex;

    unsigned int temp = writeFBO;
    writeFBO = readFBO;
    readFBO = temp;
  }
  mShaderPassThrough->Bind();
  mScreenQuad.Draw(readTex);
}

void Renderer::Clear() {
  GLCall(glClearColor(0.0f, 0.2f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                 GL_STENCIL_BUFFER_BIT));
}

void Renderer::DrawOpaque(Scene &scene) {
  for (Renderable *object : scene.opaqueObjects) {
    Shader *shader = mRegularShaderRegistry[object->type];
    shader->Bind();
    // Light calulcation
    std::vector<Light *> lights =
        scene.getClosestLights(object->transform->GetPosition(), MAX_LIGHTS);

    // Changing light uniforms
    for (unsigned int i = 0; i < lights.size(); i++) {
      shader->SetUniformVec3f("uLights[" + std::to_string(i) + "].position",
                              lights[i]->transform->GetPosition());

      // Diffuse / specular
      shader->SetUniformVec3f("uLights[" + std::to_string(i) + "].diffuse",
                              lights[i]->diffuse);

      shader->SetUniformVec3f("uLights[" + std::to_string(i) + "].specular",
                              lights[i]->specular);

      // Lights distance variables
      shader->SetUniform1f("uLights[" + std::to_string(i) + "].constant",
                           lights[i]->constant);

      shader->SetUniform1f("uLights[" + std::to_string(i) + "].linear",
                           lights[i]->linear);

      shader->SetUniform1f("uLights[" + std::to_string(i) + "].quadratic",
                           lights[i]->quadratic);
    }
    if (lights.size() < MAX_LIGHTS) {
      for (unsigned int i = lights.size(); i < MAX_LIGHTS; i++) {
        shader->SetUniformVec3f("uLights[" + std::to_string(i) + "].position",
                                glm::vec3(0.0f));

        // Ambient / diffuse / specular
        shader->SetUniformVec3f("uLights[" + std::to_string(i) + "].diffuse",
                                glm::vec3(0.0f));
        shader->SetUniformVec3f("uLights[" + std::to_string(i) + "].specular",
                                glm::vec3(0.0f));

        // Lights distance variables
        shader->SetUniform1f("uLights[" + std::to_string(i) + "].constant",
                             1.0f);

        shader->SetUniform1f("uLights[" + std::to_string(i) + "].linear", 1.0f);

        shader->SetUniform1f("uLights[" + std::to_string(i) + "].quadratic",
                             1.0f);
      }
    }

    object->Draw(*shader);
  }
}

void Renderer::DrawTransparent(Scene &scene, const glm::vec3 &cameraPosition) {
  // Sort transparent
  std::multimap<float, Renderable *> sorted;
  for (unsigned int i = 0; i < scene.transparentObjects.size(); i++) {
    float distance = glm::length(
        cameraPosition - scene.transparentObjects[i]->transform->GetPosition());
    sorted.insert(std::make_pair(distance, scene.transparentObjects[i]));
  }

  // Draw in sorted order
  for (std::multimap<float, Renderable *>::reverse_iterator it =
           sorted.rbegin();
       it != sorted.rend(); ++it) {
    it->second->Draw(*mRegularShaderRegistry[it->second->type]);
  }
}

// CubeMaps
void Renderer::AddCubeMap(const std::string &name, CubeMap *cubeMap) {
  mCubeMapsRegistry[name] = cubeMap;
}

void Renderer::RemoveCubeMap(const std::string &name) {
  mCubeMapsRegistry.erase(name);
}

void Renderer::SetActiveCubeMap(const std::string &name) {
  if (mCubeMapsRegistry.find(name) != mCubeMapsRegistry.end()) {
    mActiveCubeMap = name;
  } else {
    std::cout << "Cube map named: " << name << ", doesn't exists" << std::endl;
  }
}

void Renderer::DrawCubeMap(const glm::mat4 &viewMatrix,
                           const glm::mat4 &projMatrix) {
  if (mCubeMapsRegistry.find(mActiveCubeMap) != mCubeMapsRegistry.end()) {
    mCubeMapsRegistry[mActiveCubeMap]->Draw(viewMatrix, projMatrix);
  }
}
