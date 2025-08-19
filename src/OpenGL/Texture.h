#pragma once
#include <string>

#define T_DIFFUSE "diffuse"
#define T_SPECULAR "specular"

class Texture 
{
private:
    unsigned int m_RendererID;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
public:
    std::string filePath;
    std::string type;
    
    Texture(
        const std::string& path,
        const std::string& type
    );

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void Delete() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return  m_Height; }
};