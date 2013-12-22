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

#define PRIORTY_LAST -1

class ModManager : public Urho3D::Object
{
    OBJECT(ModManager)

public:
    ModManager(Urho3D::Context* context);

    void Load();
    void Save();

    bool Activate(const Urho3D::String& id, unsigned int priorty = PRIORTY_LAST);
    bool Deactivate(const Urho3D::String& id);

    const Urho3D::HashMap<Urho3D::String, Mod>& GetModDescriptors() const;
    bool IsActive(Urho3D::String id) const;

private:
    Urho3D::Vector<Urho3D::String> m_ActiveMods;
    Urho3D::HashMap<Urho3D::String,Mod> m_ModDescriptors;
};

