////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "State.h"

#include <UIElement.h>
#include <Timer.h>
#include <Ptr.h>
#include <Text.h>
#include <Str.h>
#include <BorderImage.h>
#include <ResourceCache.h>
#include <Vector.h>

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

private:
    void SwitchToMenu();
    void AsyncLoadCoreData();

    Urho3D::BorderImage* CreateLaunchLogo(Urho3D::ResourceCache* rc, const Urho3D::String& textureName);

    void HandleTimer(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleKey(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleButton(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    Urho3D::SharedPtr<Urho3D::UIElement> launchRoot_;
    Urho3D::Vector<Urho3D::SharedPtr<Urho3D::BorderImage>> launchLogos_;
    Urho3D::Timer timer_;
    unsigned currentLogoIndex_;
};

