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
#include <ValueAnimation.h>

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
    void SwitchLogo();
    void AsyncLoadCoreData();
    void FadeOutFinalLogo();

    Urho3D::BorderImage* CreateLaunchLogo(Urho3D::ResourceCache* rc, const Urho3D::String& textureName);

    void HandleTimer(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleKey(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleButton(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleAnimationFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    Urho3D::SharedPtr<Urho3D::UIElement> launchRoot_;
    Urho3D::Vector<Urho3D::SharedPtr<Urho3D::BorderImage>> launchLogos_;
    Urho3D::SharedPtr<Urho3D::ValueAnimation> opacityOutAnimation_;
    Urho3D::SharedPtr<Urho3D::ValueAnimation> opacityInAnimation_;
    Urho3D::Timer timer_;
    unsigned currentLogoIndex_;
    bool fading_;
};

