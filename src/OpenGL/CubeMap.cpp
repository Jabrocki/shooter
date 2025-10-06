#include "CubeMap.h"

// STL
#include <string>

// OpenGL
#include "Debug.h"
#include "VertexBufferLayout.h"

// External
#include <stb_image/stb_image.h>

float skyboxVertices[] = {
    // Coordinates
    -1.0f, -1.0f, 1.0f,  //
    1.0f,  -1.0f, 1.0f,  //
    1.0f,  -1.0f, -1.0f, //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, 1.0f,  1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  1.0f,  -1.0f, //
    -1.0f, 1.0f,  -1.0f,
};

// 36 indices (12 triangles) — includes top & bottom faces
unsigned int skyboxIndices[] = {
    // +X
    1, 2, 6, 6, 5, 1,
    // -X
    0, 4, 7, 7, 3, 0,
    // +Y (top)
    4, 5, 6, 6, 7, 4,
    // -Y (bottom)
    0, 3, 2, 2, 1, 0,
    // +Z (front)
    0, 1, 5, 5, 4, 0,
    // -Z (back)
    3, 7, 6, 6, 2, 3};

std::string cubeMapShaderSourceVert = R"(
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 uView; 
uniform mat4 uProj;

void main()
{
    vec4 pos = uProj * uView * vec4(aPos, 1.0);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    TexCoords = vec3(aPos.x, aPos.y, -aPos.z);
}
)";

std::string cubeMapShaderSourceFrag = R"(
#version 330 core 

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
void main()
{    
    FragColor = texture(skybox, TexCoords);
}
)";

CubeMap::CubeMap(const std::string &rightPath, const std::string &leftPath,
                 const std::string &topPath, const std::string &bottomPath,
                 const std::string &frontPath, const std::string &backPath)
    : mVBO(skyboxVertices, sizeof(skyboxVertices)),
      mIBO(skyboxIndices, sizeof(skyboxIndices)),
      mShader(Shader::fromSource(cubeMapShaderSourceVert,
                                 cubeMapShaderSourceFrag)) {
  mVAO.Bind();
  mLayout.Push<float>(3);
  mVAO.AddBuffer(mVBO, mLayout);
  mIBO.Bind();

  GLCall(glGenTextures(1, &mRendererId));
  GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererId));

  // Texture parameters
  GLCall(
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                         GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                         GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                         GL_CLAMP_TO_EDGE));

  // Load each face
  std::string faces[6] = {rightPath,  leftPath,  topPath,
                          bottomPath, frontPath, backPath};
  // Cube maps are loaded the other way around
  //
  stbi_set_flip_vertically_on_load(false);
  for (unsigned int i = 0; i < 6; i++) {
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                          height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap texture failed to load at path: " << faces[i]
                << std::endl;
      stbi_image_free(data);
    }
  }

  mVAO.UnBind();
}

void CubeMap::Draw(glm::mat4 view, glm::mat4 proj) {

  GLCall(glDepthFunc(GL_LEQUAL));
  GLCall(glDepthMask(GL_FALSE));
  GLCall(glDisable(GL_CULL_FACE));

  view = glm::mat4(glm::mat3(view));
  mShader.Bind();
  mShader.SetUniformMat4f("uView", view);
  mShader.SetUniformMat4f("uProj", proj);
  mShader.SetUniform1i("skybox", 0);

  mVAO.Bind();

  GLCall(glActiveTexture(GL_TEXTURE0));
  GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererId));
  GLCall(glDrawElements(GL_TRIANGLES,
                        sizeof(skyboxIndices) / sizeof(unsigned int),
                        GL_UNSIGNED_INT, 0));

  mVAO.UnBind();

  GLCall(glEnable(GL_CULL_FACE));
  GLCall(glDepthMask(GL_TRUE));
  GLCall(glDepthFunc(GL_LESS));
}
