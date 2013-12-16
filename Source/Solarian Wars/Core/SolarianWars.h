#pragma once

#include <Engine.h>
#include <Application.h>

class SolarianWars : public Urho3D::Application
{
    OBJECT(SolarianWars)

public:
    SolarianWars(Urho3D::Context* context);

    virtual void Setup();
    virtual void Start();
    virtual void Stop();
};

