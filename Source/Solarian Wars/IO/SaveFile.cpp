////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Core\Events.h"
#include "SaveFile.h"

#include <File.h>
#include <Scene.h>

using namespace Urho3D;

SaveFile::SaveFile(Context* context) :
Object(context),
m_Header(SaveHeader())
{
}

bool SaveFile::LoadHeader(const String& source)
{
    File file(context_, source);

    if (file.IsOpen())
    {
        file.Read(m_Header.characterName, sizeof(SaveHeader::characterName));
        file.Read(m_Header.corporationName, sizeof(SaveHeader::corporationName));
        file.Read(m_Header.currentSystem, sizeof(SaveHeader::currentSystem));
        file.Read(m_Header.thumbnail, sizeof(SaveHeader::thumbnail));
    }
}

bool SaveFile::Load(const String& source)
{
    File file(context_, source);
    if (file.IsOpen())
    {
        //Move past the header
        file.Seek(sizeof(SaveHeader));

        //TODO: 1. Need to display loading state
        //TODO: 2. Start the Scene Loading Async
        //TODO: 3. Create the GalaxyMap State
        //TODO: 4. Loading State needs to check if scene async if completed and then hide itself and set the galaxy state active
    }
}

bool SaveFile::Save(const String& dest, Scene* scene) const
{
    if (scene)
    {
        File file(context_, dest, FILE_WRITE);

        if (file.IsOpen())
        {
            file.Write(m_Header.characterName, sizeof(SaveHeader::characterName));
            file.Write(m_Header.corporationName, sizeof(SaveHeader::corporationName));
            file.Write(m_Header.currentSystem, sizeof(SaveHeader::currentSystem));
            file.Write(m_Header.thumbnail, sizeof(SaveHeader::thumbnail));

            return scene->Save(file);
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
