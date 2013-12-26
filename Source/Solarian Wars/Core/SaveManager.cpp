////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "SaveManager.h"
#include "IO/Settings.h"

#include <FileSystem.h>
#include <File.h>
#include <ForEach.h>
#include <Log.h>
#include <Timer.h>
#include <Graphics.h>
#include <UI.h>

using namespace Urho3D;

SaveManager::SaveManager(Context* context) :
    Object(context)
{
}

void SaveManager::LoadHeaders()
{
    saveHeaders_.Clear();

    String saveDir = GetSubsystem<Settings>()->GetSetting("userdir").GetString() + "Saves/";

    Vector<String> saveHeaders;
    GetSubsystem<FileSystem>()->ScanDir(saveHeaders, saveDir, "*.sws", SCAN_FILES, false);
    foreach(String file, saveHeaders)
    {
        SaveHeader save(context_);
        if (save.Load(saveDir + file))
        {
            saveHeaders_[save.GetName()] = save;
        }
        else
        {
            LOGERROR("Unable to load save header for " + file);
        }
    }
}

const Urho3D::HashMap<String,SaveHeader>& SaveManager::GetHeaders()
{
    return saveHeaders_;
}

bool SaveManager::LoadGame(const Urho3D::String& source)
{
    String saveDir = GetSubsystem<Settings>()->GetSetting("userdir").GetString() + "Saves/";
    File file(context_, saveDir + source + ".sws");

    if (file.IsOpen())
    {
        //Move past the header
        file.Seek(saveHeaders_[source].GetLength());

        /* TODO:
        * 1. Need to display loading state
        * 2. Start the Scene Loading Async
        * 3. Create the GalaxyMap State
        * 4. Load the UI 
        * 5. Loading State needs to check if scene async if completed and then hide itself and set the galaxy state active
        */
    }

    return false;
}

bool SaveManager::SaveGame(const Urho3D::String& dest, Urho3D::Scene* scene, const Urho3D::String& character, const Urho3D::String& corporation, const Urho3D::String& system)
{
    String saveDir = GetSubsystem<Settings>()->GetSetting("userdir").GetString() + "Saves/";
    File file(context_, saveDir + dest + ".sws", FILE_WRITE);

    if (file.IsOpen())
    {
        if (WriteSaveHeader(file, character, corporation, system))
        {
            return WriteScene(file, scene);
        }
    }

    return false;
}

bool SaveManager::WriteSaveHeader(Urho3D::File& dest, const Urho3D::String& character, const Urho3D::String& corporation, const Urho3D::String& system)
{
    if (saveHeaders_.Contains(dest.GetName()))
    {
        SaveHeader& header = saveHeaders_[dest.GetName()];
        header.SetDateCreated(GetSubsystem<Time>()->GetSystemTime());
        header.SetLength(0);
        header.SetCharacterName(character);
        header.SetCorporationName(corporation);
        header.SetCurrentSystem(system);
        header.SetPlayedTime(GetSubsystem<Time>()->GetElapsedTime());

        Image* thumbnail = new Image(context_);
        GetSubsystem<UI>()->GetRoot()->SetVisible(false);
        GetSubsystem<Graphics>()->TakeScreenShot(*thumbnail);
        GetSubsystem<UI>()->GetRoot()->SetVisible(true);

        thumbnail->SetSize(128, 128, 3);
        header.SetThumbnail(thumbnail);

        return header.Save(dest);
    }
    else
    {
        SaveHeader header = SaveHeader(context_);
        header.SetName(dest.GetName());
        header.SetDateCreated(GetSubsystem<Time>()->GetSystemTime());
        header.SetCharacterName(character);
        header.SetCorporationName(corporation);
        header.SetCurrentSystem(system);
        header.SetPlayedTime(GetSubsystem<Time>()->GetElapsedTime());

        Image* thumbnail = new Image(context_);
        GetSubsystem<UI>()->GetRoot()->SetVisible(false);
        GetSubsystem<Graphics>()->TakeScreenShot(*thumbnail);
        GetSubsystem<UI>()->GetRoot()->SetVisible(true);

        thumbnail->SetSize(128, 128, 3);
        header.SetThumbnail(thumbnail);

        return header.Save(dest);
    }
}

bool SaveManager::WriteScene(Urho3D::File& dest, Urho3D::Scene* scene)
{
    return scene->Save(dest);
}
