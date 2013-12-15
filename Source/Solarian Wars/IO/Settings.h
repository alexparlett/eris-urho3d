#pragma once

#include <Object.h>
#include <Str.h>
#include <XMLElement.h>
#include <Variant.h>

using namespace Urho3D;

class Settings : public Object
{
	OBJECT(Settings)

public:
	Settings(Context* context);

	void Load();
	void Save();

	Variant GetSetting(const String& name) const;
	void SetSetting(const String& name, const String& value);

private:
	const String GetUserDirectory();
	void LoadUserSettings(const String& fileName);
	void LoadDefaults();
	void LoadFromXml(const XMLElement& root);
	
	void LoadGraphics(const XMLElement& graphics);
	void LoadSound(const XMLElement& sound);

	void SaveGraphics(XMLElement& root);
	void SaveSound(XMLElement& root);

	VariantMap m_Settings;
};

