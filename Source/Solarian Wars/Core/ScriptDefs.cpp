////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "ScriptDefs.h"
#include "IO/Locale.h"
#include "IO/Settings.h"

#include "GamePlay/Map/Types/Galaxy.h"
#include "GamePlay/Map/Types/System.h"

#include "GamePlay/Map/Properties/SystemProperties.h"

#include <APITemplates.h>
#include <angelscript.h>
#include <stddef.h>

using namespace Urho3D;

static void LocaleReplacePODVector(String& line, CScriptArray* tokens, Locale* ptr)
{
    ptr->Replace(line, ArrayToVector<String>(tokens));
}

static Locale* GetLocale()
{
    return GetScriptContext()->GetSubsystem<Locale>();
}

static Settings* GetSettings()
{
    return GetScriptContext()->GetSubsystem<Settings>();
}

static void RegisterSettings(asIScriptEngine* engine)
{
    RegisterObject<Settings>(engine, "Settings");
    engine->RegisterObjectMethod("Settings", "const Variant& GetSetting(const String&in, const Variant&in = Variant()) const", asMETHOD(Settings, GetSetting), asCALL_THISCALL);
    engine->RegisterObjectMethod("Settings", "void SetSetting(const String&in, const Variant&in)", asMETHOD(Settings, SetSetting), asCALL_THISCALL);
    engine->RegisterGlobalFunction("Settings@+ get_settings()", asFUNCTION(GetSettings), asCALL_CDECL);
}

static Galaxy* GetGalaxy()
{
	return GetScriptContext()->GetSubsystem<Scene>()->GetComponent<Galaxy>();
}

static void RegisterLocale(asIScriptEngine* engine)
{
    RegisterRefCounted<Locale>(engine, "Locale");
    engine->RegisterObjectMethod("Locale", "String Localize(int,int) const", asMETHOD(Locale, Localize), asCALL_THISCALL);
    engine->RegisterObjectMethod("Locale", "void Replace(String&in, int, const String&in) const", asMETHODPR(Locale, Replace, (Urho3D::String& line, int token, const Urho3D::String& value) const, void), asCALL_THISCALL);
    engine->RegisterObjectMethod("Locale", "void Replace(String&in, Array<String>@+) const", asFUNCTION(LocaleReplacePODVector), asCALL_CDECL_OBJLAST);
    engine->RegisterGlobalFunction("Locale@+ get_locale()", asFUNCTION(GetLocale), asCALL_CDECL);
}

static void RegisterProperties(asIScriptEngine* engine)
{
    engine->RegisterObjectType("SystemProperties", sizeof(SystemProperties), asOBJ_VALUE | asOBJ_POD);
    engine->RegisterObjectProperty("SystemProperties", "String name", offsetof(SystemProperties, name_));
    engine->RegisterObjectProperty("SystemProperties", "Vector3 position", offsetof(SystemProperties, position_));
	engine->RegisterObjectProperty("SystemProperties", "String controller", offsetof(SystemProperties,  controller_));
}

static void RegisterTypes(asIScriptEngine* engine)
{
    RegisterComponent<System>(engine, "System");

	RegisterComponent<Galaxy>(engine, "Galaxy");
	engine->RegisterObjectMethod("Galaxy", "System@+ CreateSystem(const SystemProperties&in)", asMETHOD(Galaxy, CreateSystem), asCALL_THISCALL);
	engine->RegisterObjectMethod("Galaxy", "System@+ GetSystem(const String&in) const", asMETHOD(Galaxy, GetSystem), asCALL_THISCALL);
	engine->RegisterGlobalFunction("Galaxy@+ get_galaxy()", asFUNCTION(GetGalaxy), asCALL_CDECL);
}

void RegisterScriptAPI(asIScriptEngine* engine)
{
    RegisterLocale(engine);
    RegisterSettings(engine);
	RegisterProperties(engine);
    RegisterTypes(engine);
}