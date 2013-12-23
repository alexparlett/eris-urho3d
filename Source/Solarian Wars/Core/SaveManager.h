////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "IO/SaveFile.h"

#include <Object.h>
#include <Vector.h>

class SaveManager :  public Urho3D::Object
{
    OBJECT(SaveManager)

public:
    SaveManager(Urho3D::Context* context);

    void LoadHeaders();
    const Urho3D::Vector<SaveHeader>&  GetHeaders();

    void GameSaved(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::Vector<SaveHeader> m_SaveHeaders;
};

