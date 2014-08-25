////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "Core\State.h"

#include <UIElement.h>
#include <Timer.h>
#include <Ptr.h>

class LaunchState : public State
{
    OBJECT(LaunchState)
    BASEOBJECT(State)

public:
    LaunchState(Urho3D::Context* context);
    virtual ~LaunchState();

    virtual void Create();
    virtual void Start();
    virtual void Stop();
    virtual void Destroy();

    void HandleTimer(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleKey(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::SharedPtr<Urho3D::UIElement> loading_;
    Urho3D::Timer timer_;
};

