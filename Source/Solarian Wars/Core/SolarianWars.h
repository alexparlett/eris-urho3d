////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Engine.h>
#include <Application.h>
#include <Vector2.h>

class SolarianWars : public Urho3D::Application
{
    OBJECT(SolarianWars)

public:
    SolarianWars(Urho3D::Context* context);

    virtual void Setup();
    virtual void Start();
    virtual void Stop();
};