#pragma once

#include "../Components/Mesh.h"
#include "Renderable.h"

class MeshRenderable : public Renderable {
public:
  MeshRenderable(const std::string &objType, Mesh mesh)
      : mesh(mesh), Renderable(objType) {};

  Mesh mesh;
  void Draw(Shader &shader) override {
    mesh.Draw(shader, transform->GetModelMatrix());
  }
};
