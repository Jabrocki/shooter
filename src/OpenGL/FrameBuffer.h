#pragma once

class FrameBuffer {
public:
  FrameBuffer(
    float scr_width,
    float scr_height
  );

  void Bind() const;
  void UnBind() const;
  unsigned int GetTextureID() const { return m_TextureID; }

private:
  unsigned int m_RendererID;
  unsigned int m_TextureID;
  unsigned int m_RBO;
};
