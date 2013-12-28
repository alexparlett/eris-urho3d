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
    engineParameters_["WindowTitle"] = "Solarian Wars Editor";
    engineParameters_["WindowIcon"] = "Textures/Icon.png";
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/editor.log";
    engineParameters_["FullScreen"] = false;
    engineParameters_["WindowResizable"] = true;
    engineParameters_["WindowWidth"] = 0;
    engineParameters_["WindowHeight"] = 0;

    if (production_)
    {
        engineParameters_["ResourcePaths"] = "CoreData;Data;../Assets/Editor";
    }
    else
    {
        engineParameters_["ResourcePaths"] = "CoreData;Data;Editor";
    }
}

void Editor::Start()
{
    scriptFile_ = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>("Scripts/Editor.as");

    VariantVector parameters;
    parameters.Push(production_);
    parameters.Push(version);

    // If script loading is successful, proceed to main loop
    if (scriptFile_ && scriptFile_->Execute("void Start(bool,String)", parameters))
    {
        // Subscribe to script's reload event to allow live-reload of the application
        SubscribeToEvent(scriptFile_, E_RELOADSTARTED, HANDLER(Editor, HandleScriptReloadStarted));
        SubscribeToEvent(scriptFile_, E_RELOADFINISHED, HANDLER(Editor, HandleScriptReloadFinished));
        SubscribeToEvent(scriptFile_, E_RELOADFAILED, HANDLER(Editor, HandleScriptReloadFailed));
        return;
    }

    // The script was not successfully loaded. Show the last error message and do not run the main loop
    ErrorExit();
}

void Editor::Stop()
{
    if (scriptFile_)
    {
        // Execute the optional stop function
        if (scriptFile_->GetFunction("void Stop()"))
            scriptFile_->Execute("void Stop()");
    }
}

void Editor::HandleScriptReloadStarted(StringHash eventType, VariantMap& eventData)
{
    if (scriptFile_->GetFunction("void Stop()"))
        scriptFile_->Execute("void Stop()");
}

void Editor::HandleScriptReloadFinished(StringHash eventType, VariantMap& eventData)
{
    VariantVector parameters;
    parameters.Push(production_);
    parameters.Push(version);

    if (!scriptFile_->Execute("void Start(bool,String)",parameters))
    {
        scriptFile_.Reset();
        ErrorExit();
    }
}

void Editor::HandleScriptReloadFailed(StringHash eventType, VariantMap& eventData)
{
    scriptFile_.Reset();
    ErrorExit();
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