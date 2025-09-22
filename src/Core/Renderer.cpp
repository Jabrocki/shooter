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

Renderer::Renderer()
    : mUBO([&] {
        UniformBufferLayout uboLayout;
        uboLayout.Push<glm::mat4>("uView");
        uboLayout.Push<glm::mat4>("uProjection");
        uboLayout.Push<glm::vec3>("uViewPosition");
        return UniformBuffer(uboLayout);
      }()),
      mFBO(800.0f, 600.0f) // TODO: dynamic screen size
{
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

  // Frame buffer init
  float quadVertices[] = {// vertex attributes for a quad that fills the entire
                          // screen in Normalized Device Coordinates.
                          // positions   // texCoords
                          -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                          0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                          -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                          1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));

  mScreenShader.Bind();
  mScreenShader.SetUniform1i("screenTexture", 0);
}

void Renderer::AddShader(std::string &objType, Shader *shader) {
  mShaderRegistry[objType] = shader;
}

void Renderer::Draw(Scene &scene, const glm::mat4 &viewMatrix,
                    const glm::mat4 &projMatrix,
                    const glm::vec3 &cameraPostion) {
  mFBO.Bind();
  // Set global uniforms
  mUBO.SetUniformMat4f("uView", viewMatrix);
  mUBO.SetUniformMat4f("uProjection", projMatrix);
  mUBO.SetUniformVec3f("uViewPosition", cameraPostion);

  DrawOpaque(scene);
  DrawTransparent(scene, cameraPostion);

  mFBO.UnBind();

  mScreenShader.Bind();
  GLCall(glDisable(GL_DEPTH_TEST));
  GLCall(glDisable(GL_CULL_FACE)); // bezpiecznie dla full-screen quad
  GLCall(glActiveTexture(GL_TEXTURE0));
  GLCall(glBindTexture(GL_TEXTURE_2D, mFBO.GetTextureID())); // <<< klucz
  GLCall(glBindVertexArray(quadVAO));
  GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void Renderer::Clear() {
  GLCall(glClearColor(0.0f, 0.2f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                 GL_STENCIL_BUFFER_BIT));
}

void Renderer::DrawOpaque(Scene &scene) {
  for (Renderable *object : scene.opaqueObjects) {
    Shader *shader = mShaderRegistry[object->type];
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
    it->second->Draw(*mShaderRegistry[it->second->type]);
  }
}
