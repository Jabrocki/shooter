#include "Mesh.h"

Mesh::Mesh(
    std::vector<Vertex> vertices,
    std::vector<GLuint> indices,
    std::vector<Texture> textures)
    : vertices(vertices), 
      indices(indices), 
      textures(textures),
      m_IBO(indices.data(), indices.size()),
      m_VBO(vertices.data(), vertices.size() * sizeof(Vertex))
{

    m_VAO.Bind();

    VertexBufferLayout layout;

    layout.Push<float>(3); // position
    layout.Push<float>(3); // normals
    layout.Push<float>(2); // texture coordinates

    m_VAO.AddBuffer(m_VBO, layout);

    m_VAO.UnBind();
    m_VBO.Unbind();
    m_IBO.Unbind();
}

void Mesh::Draw(Shader& shader, const glm::mat4& model)
{
    shader.Bind();
    m_VAO.Bind();
    m_VBO.Bind();
    m_IBO.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string num;
        std::string type = textures[i].type;

        if (type == T_DIFFUSE)
            num = std::to_string(numDiffuse++);
        else
            num = std::to_string(numSpecular++);
        textures[i].Bind(i);
    }
    shader.SetUniformMat4f("uModel", model);
    
    GLCall(glDrawElements(GL_TRIANGLES, m_IBO.GetCount(), GL_UNSIGNED_INT, 0));

    // Unbind textures
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        textures[i].Unbind();
    }
}