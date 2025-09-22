#pragma once

//GameObjects
#include "../Objects/Cube.h"


#include "../../Core/Scene.h"

#include <memory>

class Level : public Scene
{
public:
    std::vector<std::unique_ptr<Cube>> cubes;

    void pushCube(Transform transform)
    {
        cubes.emplace_back(std::make_unique<Cube>(transform));
        Cube* c = cubes.back().get();                 
        opaqueObjects.push_back(&c->renderable);      
    }
};