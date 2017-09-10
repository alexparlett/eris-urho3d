////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Bindings.h"
#include "Settings.h"

#include <IO/FileSystem.h>
#include <IO/File.h>
#include <IO/Log.h>
#include <Core/Context.h>
#include <Resource/XMLFile.h>
#include <Container/ForEach.h>

using namespace Urho3D;

Bindings::Bindings(Context* context) :
Object(context),
input_(context->GetSubsystem<Input>())
{

}

void Bindings::Load()
{
    Settings* settings = GetSubsystem<Settings>();

    String fileName = settings->GetSetting("userdir").GetString() + "bindings.xml";
    File file(context_);

    if (GetSubsystem<FileSystem>()->FileExists(fileName))
        file.Open(fileName);
    else
        file.Open("bindings.xml");


    if (!file.IsOpen())
    {
		URHO3D_LOGERROR("Unable to open bindings file " + fileName);
        return;
    }

	XMLFile* xmlFile = new XMLFile(context_);
    if (xmlFile->Load(file))
    {
        XMLElement root = xmlFile->GetRoot();
        XPathResultSet results = root.Select("action");

        for (unsigned i = 0; i < results.Size(); i++)
        {
            XMLElement action = results[i];
            actionMap_[action.GetAttribute("name")] = ToInt(action.GetAttribute("code"));
        }
    }
	delete(xmlFile);
}

void Bindings::Save()
{
    Settings* settings = GetSubsystem<Settings>();

    String fileName = settings->GetSetting("userdir").GetString() + "bindings.xml";

    File* file = new File(context_, fileName, FILE_WRITE);
    if (file->IsOpen())
    {
        XMLFile* xmlFile = new XMLFile(context_);
        XMLElement root = xmlFile->CreateRoot("bindings");

        HashMap<String, int>::Iterator iter = actionMap_.Begin();
        for (iter; iter != actionMap_.End(); iter++)
        {
            XMLElement action = root.CreateChild("action");
            action.SetString("name", iter->first_);
            action.SetInt("code", iter->second_);
        }

        if (!xmlFile->Save(*file))
			URHO3D_LOGERROR("Unable to save " + fileName);

		delete(xmlFile);
    }
    else
		URHO3D_LOGERROR("Unable to open " + fileName);

	delete(file);
}

int Bindings::GetActionScanCode(const String& action) const
{
    HashMap<String, int>::ConstIterator findIt = actionMap_.Find(action);
    if (findIt != actionMap_.End())
    {
        return findIt->second_;
    }

    return SCANCODE_UNKNOWN;
}

void Bindings::SetActionScanCode(const String& action, int scanCode)
{
    actionMap_[action] = scanCode;
}

void Bindings::SetActionScanCodeFromKey(const Urho3D::String& action, int keyCode)
{
    SetActionScanCode(action, input_->GetScancodeFromKey(keyCode));
}
