////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <Variant.h>
#include <HashMap.h>
#include <Str.h>

class Settings : public Urho3D::Object
{
    OBJECT(Settings)
public:
    Settings(Urho3D::Context* context);

    bool Load();
    bool Save();

    const Urho3D::Variant& GetSetting(const Urho3D::String& name, const Urho3D::Variant& default = Urho3D::Variant::EMPTY) const;
    void SetSetting(const Urho3D::String& name, const Urho3D::Variant& value);

private:
    Urho3D::VariantMap m_Settings;
};

