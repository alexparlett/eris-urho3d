////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "ScriptAPI.h"

#include <AngelScript/APITemplates.h>
#include <AngelScript/angelscript.h>
#include <AngelScript/Script.h>

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

static void RegisterUserInterface(asIScriptEngine* engine)
{
    engine->RegisterInterface("UserInterface");
    engine->RegisterInterfaceMethod("UserInterface", "void Create()");
}

void RegisterGameAPI(asIScriptEngine* engine)
{
    RegisterGlobalFunctions(engine);
    RegisterMap(engine);
    RegisterUserInterface(engine);
}