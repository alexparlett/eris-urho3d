////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "MapGenerator.h"

#include <angelscript.h>

using namespace Urho3D;

MapGenerator::MapGenerator(ScriptFile* scriptFile) :
scriptFile_(scriptFile)
{
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Generate()
{
    asIScriptObject* obj = scriptFile_->CreateObject("Map", true);
    if (obj)
    {
        asIScriptFunction* run = scriptFile_->GetMethod(obj, "void Generate()");
        if (run)
            scriptFile_->Execute(obj, run);
    }
}
