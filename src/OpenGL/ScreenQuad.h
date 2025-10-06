#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class ScreenQuad {
public:
  ScreenQuad();
  ~ScreenQuad() {};
  void Draw(unsigned int textureID) const;

private:
  VertexArray mVAO;
  VertexBuffer mVBO;
  VertexBufferLayout mLayout;
};
