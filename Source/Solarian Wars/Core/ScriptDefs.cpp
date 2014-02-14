////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "ScriptDefs.h"
#include "IO/Locale.h"

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

static void RegisterLocale(asIScriptEngine* engine)
{
    RegisterObject<Locale>(engine, "Locale");
    engine->RegisterObjectMethod("Locale", "String Localize(int page,int line) const", asMETHOD(Locale, Localize), asCALL_THISCALL);
    engine->RegisterObjectMethod("Locale", "void Replace(String&in line, int token, const String&in value) const", asMETHODPR(Locale, Replace, (Urho3D::String& line, int token, const Urho3D::String& value) const, void), asCALL_THISCALL);
    engine->RegisterObjectMethod("Locale", "void Replace(String&in line, Array<String>@+ values) const", asFUNCTION(LocaleReplacePODVector), asCALL_CDECL_OBJLAST);
    engine->RegisterGlobalFunction("Locale@+ get_locale()", asFUNCTION(GetLocale), asCALL_CDECL);
}

void RegisterScriptAPI(asIScriptEngine* engine)
{
    RegisterLocale(engine);
}