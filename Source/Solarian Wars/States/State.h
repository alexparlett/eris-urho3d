////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>

class State : public Urho3D::Object
{
    OBJECT(State)
    BASEOBJECT(State)

public:
    State(Urho3D::Context* context) : Urho3D::Object(context) { }
    virtual ~State() { }

    virtual void Create() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Destroy() = 0;
};