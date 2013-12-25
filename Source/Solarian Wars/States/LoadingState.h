////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "Core\State.h"

class LoadingState : public State
{
    OBJECT(LoadingState)

public:
    LoadingState(Urho3D::Context* context);
    virtual ~LoadingState();

    virtual void Create();
    virtual void Start();
    virtual void Stop();
    virtual void Destroy();

};

