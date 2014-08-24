////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "Core\State.h"

#include <UIElement.h>
#include <Ptr.h>

class LaunchState : public State
{
    OBJECT(LaunchState)
    BASEOBJECT(State)

public:
    LaunchState(Urho3D::Context* context);
    virtual ~LaunchState();

    virtual void Create();
    virtual void Start();
    virtual void Stop();
    virtual void Destroy();

private:
    Urho3D::SharedPtr<Urho3D::UIElement> loading_;
};

