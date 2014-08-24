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
    scriptFile_->ReleaseRef();
}

void MapGenerator::Generate()
{
	asIScriptObject* obj = scriptFile_->CreateObject("MapGenerator");
	if (obj)
	{
		asIScriptFunction* execute = scriptFile_->GetMethod(obj, "void run()");
		if (execute)
		{
            scriptFile_->Execute(obj, execute);
		}
	}
}
