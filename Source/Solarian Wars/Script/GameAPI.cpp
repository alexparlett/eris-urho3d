////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "ScriptAPI.h"

#include <APITemplates.h>
#include <angelscript.h>
#include <Script.h>

using namespace Urho3D;

static bool RegisterGlobalScriptObject(const String& declaration, asIScriptObject* object)
{
    if (object)
    {

        asIScriptEngine* engine = GetScriptContext()->GetSubsystem<Script>()->GetScriptEngine();
        int result = engine->RegisterGlobalProperty(declaration.CString(), &object);

        object->Release();

        return result <= 0;
    }

    return false;
}

static void RegisterMap(asIScriptEngine* engine)
{
    engine->RegisterInterface("Map");
    engine->RegisterInterfaceMethod("Map", "void Generate()");
}

static void RegisterGlobalFunctions(asIScriptEngine* engine)
{
    engine->RegisterGlobalFunction("bool RegisterGlobalScriptObject(const String&in, ScriptObject@)", asFUNCTION(RegisterGlobalScriptObject), asCALL_CDECL);
}

void RegisterGameAPI(asIScriptEngine* engine)
{
    RegisterGlobalFunctions(engine);
    RegisterMap(engine);
}