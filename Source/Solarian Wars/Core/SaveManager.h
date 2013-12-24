////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "IO/SaveFile.h"

#include <Context.h>
#include <Scene.h>
#include <HashMap.h>

class SaveManager :  public Urho3D::Object
{
    OBJECT(SaveManager)

public:
    SaveManager(Urho3D::Context* context);

    ///Read The Save Headers
    void LoadHeaders();
    ///Get The Save Headers
    const Urho3D::HashMap<Urho3D::String,SaveHeader>&  GetHeaders();

    ///Read the Save Data
    bool LoadGame(const Urho3D::String& source);
    ///Create a new Save including Header and Data
    bool SaveGame(const Urho3D::String& dest, Urho3D::Scene* scene, const Urho3D::String& character, const Urho3D::String& corporation, const Urho3D::String& system);

private:
    bool WriteSaveHeader(Urho3D::File& dest, const Urho3D::String& character, const Urho3D::String& corporation, const Urho3D::String& system);
    bool WriteScene(Urho3D::File& dest, Urho3D::Scene* scene);

    Urho3D::HashMap<Urho3D::String,SaveHeader> m_SaveHeaders;
};

