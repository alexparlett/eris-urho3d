////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Container/Ptr.h>
#include <Scene/Node.h>
#include <Scene/Scene.h>
#include <UI/UIElement.h>
#include <Graphics/Texture2D.h>
#include <UI/Font.h>
#include <UI/Button.h>

#include "State.h"
#include "IO/Locale.h"

class MenuState : public State
{
    URHO3D_OBJECT (MenuState, State)

public:
    MenuState (Urho3D::Context* context);
    virtual ~MenuState ();

    virtual void Create();
    virtual void Start();
    virtual void Stop();
    virtual void Destroy();

private:
    void CreateCamera();
    void CreateScene();
    void CreateUI();

    void CreateButtons(Urho3D::Font* font);
    Urho3D::Button* BuildMenuButton(const Urho3D::String& content, Urho3D::Font* font);

    void PlayClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void LoadClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void ModsClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void OptionsClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void ExitClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    Urho3D::SharedPtr<Urho3D::Scene> scene_;
    Urho3D::SharedPtr<Urho3D::Node> camera_;
    Urho3D::SharedPtr<Urho3D::UIElement> menuRoot_;

    Locale* locale_;
};

