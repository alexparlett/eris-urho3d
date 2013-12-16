#pragma once

#include <Object.h>
#include <Str.h>
#include <XMLElement.h>
#include <Variant.h>

class Settings : public Urho3D::Object
{
    OBJECT(Settings)

public:
    Settings(Urho3D::Context* context);

    void Load();
    void Save();

    Urho3D::Variant GetSetting(const Urho3D::String& name, const Urho3D::Variant& default = Urho3D::Variant::EMPTY) const;
    void SetSetting(const Urho3D::String& name, const Urho3D::String& value);

private:
    const Urho3D::String GetUserDirectory();
    void LoadUserSettings(const Urho3D::String& fileName);
    void LoadDefaults();
    void LoadFromXml(const Urho3D::XMLElement& root);

    void LoadGraphics(const Urho3D::XMLElement& graphics);
    void LoadSound(const Urho3D::XMLElement& sound);

    void SaveGraphics(Urho3D::XMLElement& root);
    void SaveSound(Urho3D::XMLElement& root);

    Urho3D::VariantMap m_Settings;
};

