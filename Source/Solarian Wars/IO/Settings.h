////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////


#pragma once

#include <Core/Object.h>
#include <Container/Str.h>
#include <Resource/XMLElement.h>
#include <Core/Variant.h>

class Settings : public Urho3D::Object
{
	URHO3D_OBJECT(Settings,Urho3D::Object)

public:
    Settings(Urho3D::Context* context);

    void Load();
    void Save();

    const Urho3D::Variant& GetSetting(const Urho3D::String& name, const Urho3D::Variant& default = Urho3D::Variant::EMPTY) const;
    void SetSetting(const Urho3D::String& name, const Urho3D::Variant& value);

private:
    void LoadGraphics(const Urho3D::XMLElement& graphics);
    void LoadSound(const Urho3D::XMLElement& sound);
    void LoadGame(const Urho3D::XMLElement& game);

    void SaveGraphics(Urho3D::XMLElement& root);
    void SaveSound(Urho3D::XMLElement& root);
    void SaveGame(Urho3D::XMLElement& root);

    Urho3D::VariantMap settings_;
};

