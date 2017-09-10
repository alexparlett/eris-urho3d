////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Core/Object.h>

class State : public Urho3D::Object
{
	URHO3D_OBJECT(State, Urho3D::Object)

public:
    State(Urho3D::Context* context) : Urho3D::Object(context), switching_(false) { }
    virtual ~State() { }

    virtual void Create() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Destroy() = 0;

protected:
    bool switching_;
};