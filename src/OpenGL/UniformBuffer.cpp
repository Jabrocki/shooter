#include "UniformBuffer.h"
#include "Debug.h"
#include "iostream"

UniformBuffer::UniformBuffer(const UniformBufferLayout& layout)
    : m_Layout(layout)
{
    m_SizeBytes = m_Layout.GetBlockSize();
    IndexElements();

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_UNIFORM_BUFFER, m_SizeBytes, nullptr, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

UniformBuffer::~UniformBuffer()
{
    //GLCall(glDeleteBuffers(1, &m_RendererID));
}

void UniformBuffer::Bind()
{
    GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_RendererID));
}

void UniformBuffer::IndexElements()
{
    m_Views.clear();
    for (const UniformBufferElement& e : m_Layout.GetElements())
    {
        m_Views[e.name] = View{ e.offset, e.size, e.glType };
    }
}

void UniformBuffer::SetRaw(const std::string& name, const void* src, size_t sizeBytes)
{
    auto e = m_Views.find(name);

    if (e == m_Views.end())
    {
        std::cout << "Uniform doesn't exists: " << name << std::endl;
        return;
    }

    const View& v = e->second;

    const size_t nbytes = (sizeBytes > v.size) ? v.size : sizeBytes;

    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, v.offset, nbytes, src));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}