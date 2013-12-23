////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Core\Events.h"
#include "SaveFile.h"

#include <File.h>
#include <FileSystem.h>
#include <Scene.h>
#include <Log.h>
#include <Timer.h>

#include <time.h>

using namespace Urho3D;

SaveFile::SaveFile(Context* context) :
Object(context),
m_Header(SaveHeader())
{
}

bool SaveFile::LoadHeader(const String& source)
{
    FileSystem* fs = GetSubsystem<FileSystem>();
    File file(context_, source);

    if (file.IsOpen())
    {
        String name = file.GetName().Split('.')[0];
        memcpy(m_Header.name,name.CString(),name.Length());

        time_t t = static_cast<time_t>(fs->GetLastModifiedTime(source));
        struct tm timeinfo;
        localtime_s(&timeinfo, &t);
        strftime(m_Header.date, MAX_LENGTH, "%Y-%m-%d %H:%M:%S", &timeinfo);

        file.Read(m_Header.time, MAX_LENGTH);
        file.Read(m_Header.characterName, MAX_LENGTH);
        file.Read(m_Header.corporationName, MAX_LENGTH);
        file.Read(m_Header.currentSystem, MAX_LENGTH);
        file.Read(m_Header.thumbnail, THUMBNAIL_SIZE);

        return true;
    }

    return false;
}

bool SaveFile::Load(const String& source)
{
    FileSystem* fs = GetSubsystem<FileSystem>();
    String saveDir = fs->GetUserDocumentsDir() + "My Games/Solarian Wars/Saves/";

    File file(context_, saveDir + source + ".sws");
    if (file.IsOpen())
    {
        //Move past the header but need to ignore name and date as they are not persisted in the header
        file.Seek(sizeof(SaveHeader) - (2 * MAX_LENGTH));

        /* TODO: 
         * 1. Need to display loading state
         * 2. Start the Scene Loading Async
         * 3. Create the GalaxyMap State
         * 4. Loading State needs to check if scene async if completed and then hide itself and set the galaxy state active
         */
    }

    return false;
}

bool SaveFile::Save(const String& dest, Scene* scene, const String& character, const String& corporation, const String& system, Image* thumbnail)
{
    if (scene)
    {
        FileSystem* fs = GetSubsystem<FileSystem>();
        String saveDir = fs->GetUserDocumentsDir() + "My Games/Solarian Wars/Saves/";

        File file(context_, saveDir + dest + ".sws", FILE_WRITE);

        if (file.IsOpen())
        {
            strncpy(m_Header.characterName, character.CString(), character.Length());
            strncpy(m_Header.corporationName, corporation.CString(), corporation.Length());
            strncpy(m_Header.currentSystem, system.CString(), system.Length());
            memcpy(m_Header.thumbnail, thumbnail->GetData(), sizeof(thumbnail->GetData()));;

            time_t t = static_cast<time_t>(GetSubsystem<Time>()->GetElapsedTime());
            struct tm timeinfo;
            localtime_s(&timeinfo, &t);
            char* time;
            strftime(time, MAX_LENGTH, "%H:%M:%S", &timeinfo);
            strncpy(m_Header.time, time, sizeof(time));
            delete time;

            file.Write(m_Header.time, MAX_LENGTH);
            file.Write(m_Header.characterName, MAX_LENGTH);
            file.Write(m_Header.corporationName, MAX_LENGTH);
            file.Write(m_Header.currentSystem, MAX_LENGTH);
            file.Write(m_Header.thumbnail, THUMBNAIL_SIZE);

            bool saved = scene->Save(file);

            file.Close();

            if (saved)
            {
                SendEvent(E_GAME_SAVED);
            }
            else
            {
                LOGERROR("Unable to save file " + dest);
            }
        }
    }

    return false;
}

const SaveHeader& SaveFile::GetHeader()
{
    return m_Header;
}