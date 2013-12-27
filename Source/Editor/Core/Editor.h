////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Engine.h>
#include <Application.h>
#include <StringHash.h>
#include <Str.h>
#include <Variant.h>
#include <Ptr.h>
#include <Vector2.h>

#include "EditorScene.h"

class Editor : public Urho3D::Application
{
    OBJECT(Editor)

public:
    Editor(Urho3D::Context* context);

    virtual void Setup();
    virtual void Start();
    virtual void Stop();

private:
    void HandlerUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void ParseArguments();
    void SaveConfig();
    void LoadConfig();

    Urho3D::String m_ConfigFile;
    bool m_Production;

    Urho3D::SharedPtr<EditorScene> m_Scene;
    Urho3D::SharedPtr<EditorUI> m_UI;
};