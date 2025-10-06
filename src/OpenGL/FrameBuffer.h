#pragma once

class FrameBuffer {
public:
  FrameBuffer(float scr_width, float scr_height);

  void Bind() const;
  void UnBind() const;
  unsigned int GetTextureID() const { return m_TextureID; }
  unsigned int GetRendererID() const { return m_RendererID; }

  void Resize(float width, float height);

private:
  unsigned int m_RendererID;
  unsigned int m_TextureID;
  unsigned int m_RBO;
};
