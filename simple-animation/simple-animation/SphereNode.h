#pragma once

#include "SceneNode.h"
#include "Sphere.h"

class SphereNode : public VisitableSceneNode<SphereNode>
{
public:
    SphereNode(const glm::vec3 & position, Sphere && sphere, int accuracy = 4);

    const Sphere & getSphere() const;
    int getAccuracy() const;

private:
    Sphere mSphere;
    int mAccuracy = 5;
};

