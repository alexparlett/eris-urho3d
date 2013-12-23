////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Engine.h>
#include <Application.h>

#include <windows.h>

class Launcher : public Urho3D::Application
{
    OBJECT(Launcher)

public:
    Launcher(Urho3D::Context* context);

    virtual void Setup();
    virtual void Start();

private:
    BOOL Is64BitOS();
};

