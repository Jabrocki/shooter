#include "ScreenQuad.h"

// OpenGL
#include "Debug.h"

ScreenQuad::ScreenQuad()
    : mVBO([&] {
        float quadVertices[] = {-1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                                0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,
                                -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                                1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};
        return VertexBuffer(quadVertices, sizeof(quadVertices));
      }()) {
  mLayout.Push<float>(2); // Posistion
  mLayout.Push<float>(2); // TexCoords
  // Add buffer to VAO
  mVAO.AddBuffer(mVBO, mLayout);
}

void ScreenQuad::Draw(unsigned int textureID) const {
  GLCall(glDisable(GL_DEPTH_TEST));
  GLCall(glDisable(GL_CULL_FACE));
  GLCall(glActiveTexture(GL_TEXTURE0));
  GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
  mVAO.Bind();
  GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

  // Unbinding
  mVAO.UnBind();
  GLCall(glEnable(GL_DEPTH_TEST));
  GLCall(glEnable(GL_CULL_FACE));
}
