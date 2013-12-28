////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Engine.h>
#include <Application.h>
#include <StringHash.h>
#include <Variant.h>
#include <Ptr.h>
#include <ScriptFile.h>

class Editor : public Urho3D::Application
{
    OBJECT(Editor)

public:
    Editor(Urho3D::Context* context);

    virtual void Setup();
    virtual void Start();
    virtual void Stop();

private:
    void HandleScriptReloadFailed(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleScriptReloadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleScriptReloadStarted(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    void ParseArguments();

    /// Script file.
    Urho3D::SharedPtr<Urho3D::ScriptFile> scriptFile_;
    bool production_;
};