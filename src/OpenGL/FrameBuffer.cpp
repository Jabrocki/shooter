#include "FrameBuffer.h"
#include "Debug.h"
#include <glad/glad.h>

FrameBuffer::FrameBuffer(float scr_width, float scr_height) {
  GLCall(glGenFramebuffers(1, &m_RendererID));

  GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

  // TEST for udnerstandig
  GLCall(glGenTextures(1, &m_TextureID));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, NULL));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

  GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_2D, m_TextureID, 0));

  GLCall(glGenRenderbuffers(1, &m_RBO));
  GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));
  GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600));
  GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                   GL_RENDERBUFFER, m_RBO));
}

void FrameBuffer::Bind() const {
  GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::UnBind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
