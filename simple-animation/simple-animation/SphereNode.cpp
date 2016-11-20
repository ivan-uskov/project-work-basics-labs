#include "stdafx.h"
#include "SphereNode.h"


SphereNode::SphereNode(const glm::vec3 & position, Sphere && sphere, int accuracy)
    : VisitableSceneNode<SphereNode>(position)
    , mSphere(sphere)
    , mAccuracy(accuracy)
{
}

const Sphere & SphereNode::getSphere() const
{
    return mSphere;
}

int SphereNode::getAccuracy() const
{
    return mAccuracy;
}