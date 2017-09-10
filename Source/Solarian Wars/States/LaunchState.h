////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "State.h"

#include <UI/UIElement.h>
#include <Core/Timer.h>
#include <Container/Ptr.h>
#include <UI/Text.h>
#include <Container/Str.h>
#include <UI/BorderImage.h>
#include <Resource/ResourceCache.h>
#include <Container/Vector.h>
#include <Scene/ValueAnimation.h>

class LaunchState : public State
{
    URHO3D_OBJECT(LaunchState,State)

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

