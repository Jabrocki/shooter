#include "Cube.h"
#include "../../Core/Components/Mesh.h"
#include "../../OpenGL/Texture.h"

// 24 wierzchołki: po 4 na każdą ścianę, normalna stała na face
static const std::vector<Vertex> cubeVerts = {
// ---- front (+Z)
{{-0.5f,-0.5f, 0.5f}, {0,0, 1}, {0,0}},
{{ 0.5f,-0.5f, 0.5f}, {0,0, 1}, {1,0}},
{{ 0.5f, 0.5f, 0.5f}, {0,0, 1}, {1,1}},
{{-0.5f, 0.5f, 0.5f}, {0,0, 1}, {0,1}},
// ---- back (-Z)
{{-0.5f,-0.5f,-0.5f}, {0,0,-1}, {1,0}},
{{-0.5f, 0.5f,-0.5f}, {0,0,-1}, {1,1}},
{{ 0.5f, 0.5f,-0.5f}, {0,0,-1}, {0,1}},
{{ 0.5f,-0.5f,-0.5f}, {0,0,-1}, {0,0}},
// ---- left (-X)
{{-0.5f,-0.5f,-0.5f}, {-1,0,0}, {0,0}},
{{-0.5f,-0.5f, 0.5f}, {-1,0,0}, {1,0}},
{{-0.5f, 0.5f, 0.5f}, {-1,0,0}, {1,1}},
{{-0.5f, 0.5f,-0.5f}, {-1,0,0}, {0,1}},
// ---- right (+X)
{{ 0.5f,-0.5f,-0.5f}, { 1,0,0}, {1,0}},
{{ 0.5f, 0.5f,-0.5f}, { 1,0,0}, {1,1}},
{{ 0.5f, 0.5f, 0.5f}, { 1,0,0}, {0,1}},
{{ 0.5f,-0.5f, 0.5f}, { 1,0,0}, {0,0}},
// ---- bottom (-Y)
{{-0.5f,-0.5f,-0.5f}, {0,-1,0}, {0,1}},
{{ 0.5f,-0.5f,-0.5f}, {0,-1,0}, {1,1}},
{{ 0.5f,-0.5f, 0.5f}, {0,-1,0}, {1,0}},
{{-0.5f,-0.5f, 0.5f}, {0,-1,0}, {0,0}},
// ---- top (+Y)
{{-0.5f, 0.5f, 0.5f}, {0, 1,0}, {0,1}},
{{ 0.5f, 0.5f, 0.5f}, {0, 1,0}, {1,1}},
{{ 0.5f, 0.5f,-0.5f}, {0, 1,0}, {1,0}},
{{-0.5f, 0.5f,-0.5f}, {0, 1,0}, {0,0}},
};

static const std::vector<GLuint> cubeIdx = {
  // front (+Z)  CCW
  0,1,2,  0,2,3,
  // back  (-Z)  CCW (dla normalnej -Z)
  4,5,6,  4,6,7,
  // left  (-X)
  8,9,10, 8,10,11,
  // right (+X)
  12,13,14, 12,14,15,
  // bottom(-Y)
  16,17,18, 16,18,19,
  // top   (+Y)
  20,21,22, 20,22,23
};


Cube::Cube(Transform initial)
  : renderable(
      "regular",
      Mesh(cubeVerts, cubeIdx,
           { Texture("res/textures/cube/cube_diffuse.png", T_DIFFUSE),
             Texture("res/textures/cube/cube_specular.png", T_SPECULAR)}))
  , transform(initial)
{
  renderable.transform = &this->transform;
}

