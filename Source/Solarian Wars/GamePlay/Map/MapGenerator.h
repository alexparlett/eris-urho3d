////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Ptr.h>
#include <ScriptFile.h>
#include <ResourceCache.h>

class MapGenerator
{
public:
	MapGenerator(Urho3D::ScriptFile* scriptFile);
	virtual ~MapGenerator();

	void Generate();

private:
	Urho3D::SharedPtr<ScriptFile> scriptFile_;
};

