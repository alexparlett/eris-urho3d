////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

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

};

