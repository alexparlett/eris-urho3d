////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "SaveManager.h"
#include "Events.h"

#include <FileSystem.h>
#include <ForEach.h>
#include <Log.h>

using namespace Urho3D;

SaveManager::SaveManager(Context* context) :
Object(context),
m_SaveHeaders(Vector<SaveHeader>())
{
    SubscribeToEvent(E_GAME_SAVED, HANDLER(SaveManager, GameSaved));
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
        SaveFile save(context_);
        if (save.LoadHeader(saveDir + file))
        {
            m_SaveHeaders.Push(save.GetHeader());
        }
        else
        {
            LOGERROR("Unable to load save header for " + file);
        }
    }
}

const Urho3D::Vector<SaveHeader>& SaveManager::GetHeaders()
{
    return m_SaveHeaders;
}

void SaveManager::GameSaved(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    LoadHeaders();
}
