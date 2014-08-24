////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "ScriptDefs.h"
#include "IO/Locale.h"

#include "GamePlay/Map/Types/Galaxy.h"
#include "GamePlay/Map/Types/System.h"

#include "GamePlay/Map/Properties/SystemProperties.h"

#include <APITemplates.h>

using namespace Urho3D;

static void LocaleReplacePODVector(String& line, CScriptArray* tokens, Locale* ptr)
{
    ptr->Replace(line, ArrayToVector<String>(tokens));
}

static Locale* GetLocale()
{
    return GetScriptContext()->GetSubsystem<Locale>();
}

static Galaxy* GetGalaxy()
{
	return GetScriptContext()->GetSubsystem<Scene>()->GetComponent<Galaxy>();
}

static void RegisterLocale(asIScriptEngine* engine)
{
    RegisterObject<Locale>(engine, "Locale");
    engine->RegisterObjectMethod("Locale", "String Localize(int page,int line) const", asMETHOD(Locale, Localize), asCALL_THISCALL);
    engine->RegisterObjectMethod("Locale", "void Replace(String&in line, int token, const String&in value) const", asMETHODPR(Locale, Replace, (Urho3D::String& line, int token, const Urho3D::String& value) const, void), asCALL_THISCALL);
    engine->RegisterObjectMethod("Locale", "void Replace(String&in line, Array<String>@+ values) const", asFUNCTION(LocaleReplacePODVector), asCALL_CDECL_OBJLAST);
    engine->RegisterGlobalFunction("Locale@+ get_locale()", asFUNCTION(GetLocale), asCALL_CDECL);
}

static void RegisterProperties(asIScriptEngine* engine)
{
	RegisterRefCounted<SystemProperties>(engine, "SystemProperties");
	engine->RegisterObjectProperty("SystemProperties", "name_", sizeof(Urho3D::String));
	engine->RegisterObjectProperty("SystemProperties", "position_", sizeof(Urho3D::Vector3));
}

static void RegisterTypes(asIScriptEngine* engine)
{
	RegisterComponent<Galaxy>(engine, "Galaxy");
	engine->RegisterObjectMethod("Galaxy", "System@+ CreateSystem(const SystemProperties&in properties)", asMETHOD(Galaxy, CreateSystem), asCALL_THISCALL);
	engine->RegisterObjectMethod("Galaxy", "System@+ GetSystem(const String&in name) const", asMETHOD(Galaxy, GetSystem), asCALL_THISCALL);
	engine->RegisterGlobalFunction("Galaxy@+ get_galaxy()", asFUNCTION(GetGalaxy), asCALL_CDECL);

	RegisterComponent<System>(engine, "System");
}

void RegisterScriptAPI(asIScriptEngine* engine)
{
    RegisterLocale(engine);
	RegisterProperties(engine);
	RegisterTypes(engine);
}