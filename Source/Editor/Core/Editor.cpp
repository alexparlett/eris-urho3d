////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Editor.h"
#include "../gitversion.h"

#include <ResourceCache.h>
#include <Script.h>
#include <Log.h>
#include <ResourceEvents.h>
#include <FileSystem.h>
#include <CoreEvents.h>
#include <ForEach.h>
#include <Input.h>
#include <Str.h>
#include <Vector.h>

using namespace Urho3D;

Editor::Editor(Context* context) :
Application(context)
{
    context->RegisterSubsystem(new Script(context));
}

void Editor::Setup()
{
    ParseArguments();

    engineParameters_["Headless"] = false;
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/editor.log";

    engineParameters_["WindowTitle"] = "Solarian Wars Editor";
    engineParameters_["WindowIcon"] = "Textures/Icon.png";
    engineParameters_["FullScreen"] = false;
    engineParameters_["WindowResizable"] = true;
    engineParameters_["WindowWidth"] = 0;
    engineParameters_["WindowHeight"] = 0;

    if (production_)
    {
        engineParameters_["ResourcePaths"] = "../Assets/CoreData;Data";
    }
    else
    {
        engineParameters_["ResourcePaths"] = "00;Data";
    }
}

void Editor::Start()
{
}

void Editor::Stop()
{
}

void Editor::HandleScriptReloadStarted(StringHash eventType, VariantMap& eventData)
{
    if (scriptFile_->GetFunction("void Stop()"))
        scriptFile_->Execute("void Stop()");
}

void Editor::HandleScriptReloadFinished(StringHash eventType, VariantMap& eventData)
{
}

void Editor::HandleScriptReloadFailed(StringHash eventType, VariantMap& eventData)
{
}

void Editor::ParseArguments()
{
    Vector<String> arguments = GetArguments();

    foreach(String argument, arguments)
    {
        if (argument.Compare("-production") == 0)
        {
            production_ = true;
        }
    }
}

DEFINE_APPLICATION_MAIN(Editor)