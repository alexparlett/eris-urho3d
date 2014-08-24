////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Ptr.h>
#include <ScriptFile.h>

class MapGenerator
{
public:
	MapGenerator(Urho3D::ScriptFile* scriptFile);
	~MapGenerator();

	void Generate();

private:
    Urho3D::SharedPtr<Urho3D::ScriptFile> scriptFile_;
};

