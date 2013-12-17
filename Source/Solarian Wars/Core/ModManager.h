////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <HashMap.h>
#include <StringHash.h>
#include <Vector.h>

#include "IO\Mod.h"

class ModManager : public Urho3D::Object
{
    OBJECT(ModManager)

public:
    ModManager(Urho3D::Context* context);

    void Initialize();

    void ModActivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void ModDeactivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void ModOrderChanged(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::Vector<Urho3D::StringHash> m_ActiveMods;
    Urho3D::HashMap<Urho3D::StringHash,Mod> m_ModDescriptors;
};

