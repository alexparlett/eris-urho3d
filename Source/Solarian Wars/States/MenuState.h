////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Ptr.h>
#include <Node.h>
#include <Scene.h>

#include "State.h"

class MenuState : public State
{
    OBJECT (MenuState)
    BASEOBJECT(State)

public:
    MenuState (Urho3D::Context* context);
    virtual ~MenuState ();

    virtual void Create();
    virtual void Start();
    virtual void Stop();
    virtual void Destroy();

private:
    void CreateCamera();
    void CreateScene();
    void CreateUI();

    Urho3D::SharedPtr<Urho3D::Scene> scene_;
    Urho3D::SharedPtr<Urho3D::Node> camera_;
};

