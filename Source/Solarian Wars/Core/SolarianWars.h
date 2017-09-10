////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Engine/Engine.h>
#include <Engine/Application.h>
#include <Container/Ptr.h>
#include <UI/Text.h>

class SolarianWars : public Urho3D::Application
{
    URHO3D_OBJECT(SolarianWars,Urho3D::Application)

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