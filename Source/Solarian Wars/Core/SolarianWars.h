////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Engine.h>
#include <Application.h>
#include <Ptr.h>
#include <Text.h>

class SolarianWars : public Urho3D::Application
{
    OBJECT(SolarianWars)

public:
    SolarianWars(Urho3D::Context* context);

    virtual void Setup();
    virtual void Start();
    virtual void Stop();

private:
    void ParseArgs();
    void DefineCursor();
    void DefineVersion();

    Urho3D::SharedPtr<Urho3D::Text> version_;
};