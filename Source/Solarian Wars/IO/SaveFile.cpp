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
        strftime(m_Header.date, MAX_LENGTH, "%Y-%m-%d.%M:%S", &timeinfo);

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
        //Move past the header
        file.Seek(sizeof(SaveHeader));

        //TODO: 1. Need to display loading state
        //TODO: 2. Start the Scene Loading Async
        //TODO: 3. Create the GalaxyMap State
        //TODO: 4. Loading State needs to check if scene async if completed and then hide itself and set the galaxy state active
    }

    return false;
}

bool SaveFile::Save(const String& dest, Scene* scene)
{
    if (scene)
    {
        FileSystem* fs = GetSubsystem<FileSystem>();
        String saveDir = fs->GetUserDocumentsDir() + "My Games/Solarian Wars/Saves/";

        File file(context_, saveDir + dest + ".sws", FILE_WRITE);

        if (file.IsOpen())
        {
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

void SaveFile::SetHeader(const SaveHeader& header)
{
    m_Header = header;
}
