////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "ScriptAPI.h"
#include "IO/Locale.h"
#include "IO/Settings.h"
#include "IO/Bindings.h"

#include <APITemplates.h>
#include <angelscript.h>

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

static Bindings* GetBindings()
{
    return GetScriptContext()->GetSubsystem<Bindings>();
}

static void RegisterSettings(asIScriptEngine* engine)
{
    RegisterObject<Settings>(engine, "Settings");
    engine->RegisterObjectMethod("Settings", "const Variant& GetSetting(const String&in, const Variant&in = Variant()) const", asMETHOD(Settings, GetSetting), asCALL_THISCALL);
    engine->RegisterObjectMethod("Settings", "void SetSetting(const String&in, const Variant&in)", asMETHOD(Settings, SetSetting), asCALL_THISCALL);
    engine->RegisterGlobalFunction("Settings@+ get_settings()", asFUNCTION(GetSettings), asCALL_CDECL);
}

static void RegisterLocale(asIScriptEngine* engine)
{
    RegisterRefCounted<Locale>(engine, "Locale");
    engine->RegisterObjectMethod("Locale", "String Localize(int,int) const", asMETHOD(Locale, Localize), asCALL_THISCALL);
    engine->RegisterObjectMethod("Locale", "void Replace(String&in, int, const String&in) const", asMETHODPR(Locale, Replace, (Urho3D::String& line, int token, const Urho3D::String& value) const, void), asCALL_THISCALL);
    engine->RegisterObjectMethod("Locale", "void Replace(String&in, Array<String>@+) const", asFUNCTION(LocaleReplacePODVector), asCALL_CDECL_OBJLAST);
    engine->RegisterGlobalFunction("Locale@+ get_locale()", asFUNCTION(GetLocale), asCALL_CDECL);
}

static void RegisterBindings(asIScriptEngine* engine)
{
    RegisterObject<Bindings>(engine, "Bindings");
    engine->RegisterObjectMethod("Bindings", "int get_actionKey(const String&in)", asMETHOD(Bindings, GetActionScanCode), asCALL_THISCALL);
    engine->RegisterObjectMethod("Bindings", "void set_actionKey(const String&in, int)", asMETHOD(Bindings, SetActionScanCode), asCALL_THISCALL);
    engine->RegisterGlobalFunction("Bindings@+ get_bindings()", asFUNCTION(GetBindings), asCALL_CDECL);
}

void RegisterIOAPI(asIScriptEngine* engine)
{
    RegisterLocale(engine);
    RegisterSettings(engine);
    RegisterBindings(engine);
}