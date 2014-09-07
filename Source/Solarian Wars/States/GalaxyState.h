////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "State.h"

#include <Camera.h>
#include <Node.h>
#include <Ptr.h>

class GalaxyState : public State
{
    OBJECT(GalaxyState)
    BASEOBJECT(State)

public:
    GalaxyState(Urho3D::Context* context);
    virtual ~GalaxyState();

    virtual void Create ();
    virtual void Start ();
    virtual void Stop ();
    virtual void Destroy ();

private:
    void CreateScene();
    void CreateCamera();

	Urho3D::SharedPtr<Urho3D::Scene> scene_;
    Urho3D::SharedPtr<Urho3D::Node> camera_;
};

