////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "SaveManager.h"

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
    m_SaveHeaders.Clear();

    FileSystem* fs = GetSubsystem<FileSystem>();
    String saveDir = fs->GetUserDocumentsDir() + "My Games/Solarian Wars/Saves/";

    Vector<String> saveHeaders;
    fs->ScanDir(saveHeaders, saveDir, "*.sws", SCAN_FILES, false);
    foreach(String file, saveHeaders)
    {
        SaveHeader save(context_);
        if (save.Load(saveDir + file))
        {
            m_SaveHeaders[save.GetName()] = save;
        }
        else
        {
            LOGERROR("Unable to load save header for " + file);
        }
    }
}

const Urho3D::HashMap<String,SaveHeader>& SaveManager::GetHeaders()
{
    return m_SaveHeaders;
}

bool SaveManager::LoadGame(const Urho3D::String& source)
{
    FileSystem* fs = GetSubsystem<FileSystem>();
    String saveDir = fs->GetUserDocumentsDir() + "My Games/Solarian Wars/Saves/";
    File file(context_, saveDir + source + ".sws");

    if (file.IsOpen())
    {
        //Move past the header
        file.Seek(m_SaveHeaders[source].GetLength());

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
    FileSystem* fs = GetSubsystem<FileSystem>();
    String saveDir = fs->GetUserDocumentsDir() + "My Games/Solarian Wars/Saves/";
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
    if (m_SaveHeaders.Contains(dest.GetName()))
    {
        SaveHeader& header = m_SaveHeaders[dest.GetName()];
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
    return scene->Save(dest) && GetSubsystem<UI>()->GetRoot()->Save(dest);
}
