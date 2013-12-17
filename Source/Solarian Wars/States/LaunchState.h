////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "Core\State.h"

class LaunchState : public State
{
    OBJECT(LaunchState)

public:
    LaunchState(Urho3D::Context* context);
    virtual ~LaunchState();

    virtual void Create();
    virtual void Start();
    virtual void Stop();
    virtual void Destroy();
};

