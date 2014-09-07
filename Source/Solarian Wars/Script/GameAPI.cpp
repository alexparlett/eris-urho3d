////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "ScriptAPI.h"

#include <APITemplates.h>
#include <angelscript.h>

using namespace Urho3D;

void RegisterMap(asIScriptEngine* engine)
{
    engine->RegisterInterface("Map");
    engine->RegisterInterfaceMethod("Map", "void generate()");
}

void RegisterGameAPI(asIScriptEngine* engine)
{
    RegisterMap(engine);
}