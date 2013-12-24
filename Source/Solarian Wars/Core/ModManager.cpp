////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Events.h"
#include "ModManager.h"

#include <FileSystem.h>
#include <File.h>
#include <XMLFile.h>
#include <ForEach.h>
#include <Log.h>
#include <ResourceCache.h>

using namespace Urho3D;

ModManager::ModManager(Context* context) : 
Object(context),
m_ActiveMods(Vector<String>())
{
    SubscribeToEvent(E_MOD_ACTIVATED, HANDLER(ModManager, ModActivated));
    SubscribeToEvent(E_MOD_DEACTIVATED, HANDLER(ModManager, ModDeactivated));
    SubscribeToEvent(E_MOD_ORDER_SAVED, HANDLER(ModManager, ModOrderSaved));
}

void ModManager::Load()
{
    Vector<String> dirList;

    FileSystem* fs = GetSubsystem<FileSystem>();
    fs->ScanDir(dirList, "Mods", "", SCAN_DIRS, false);

    // Handle removing the relative dirs.
    dirList.Remove(".");
    dirList.Remove("..");

    foreach (String dir, dirList)
    {
        Vector<String> descriptorFile;
        fs->ScanDir(descriptorFile, "Mods/" + dir, "mod.xml", SCAN_FILES, false);

        if (descriptorFile.Size() > 0)
        {
            File file(context_, "Mods/" + dir + "/" + descriptorFile[0]);
            
            if (file.IsOpen())
            {
                XMLFile descriptor(context_);
                descriptor.Load(file);

                Mod mod(dir, descriptor);

                m_ModDescriptors[mod.GetId()] = mod;
            }
        }
    }

    String orderFilename = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/modorder.xml";
    if (fs->FileExists(orderFilename))
    {
        File orderFile(context_, orderFilename);
        if (orderFile.IsOpen())
        {
            XMLFile orderXml(context_);
            orderXml.Load(orderFile);

            XMLElement root = orderXml.GetRoot("mods");
            XPathResultSet mods = root.Select("mod");
            for (unsigned int i = 0; i < mods.Size(); i++)
            {
                //Required due to conventions of for loops in XPathResultSet.
                XMLElement mod = mods[i];
                String value = mod.GetValue();

                if (value != String::EMPTY && m_ModDescriptors.Contains(value))
                {
                    m_ActiveMods.Push(value);
                }
            }
        }
    }
}

void ModManager::Save()
{
    /*Release unloaded resources, this should be safe in main menu as only used resources are in CoreData.pak
     *which can not have been modified by this process
     */
    GetSubsystem<ResourceCache>()->ReleaseAllResources();

    String orderFilename = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/modorder.xml";
    File orderFile(context_, orderFilename, FILE_WRITE);
    if (orderFile.IsOpen())
    {
        XMLFile orderXml(context_);
        XMLElement root = orderXml.CreateRoot("mods");
        foreach(String id, m_ActiveMods)
        {
            root.CreateChild("mod").SetValue(id);
        }

        if (!orderXml.Save(orderFile))
        {
            LOGERROR("Unable to save mod order " + orderFilename);
        }
    }
    else
    {
        LOGERROR("Unable to open mod order " + orderFilename);
    }
}

bool ModManager::Activate(const Urho3D::String& id, unsigned int priorty)
{
    if (!m_ActiveMods.Contains(id) && m_ModDescriptors.Contains(id))
    {
        if (priorty > PRIORITY_LOW && priorty < m_ActiveMods.Size())
        {
            m_ActiveMods.Insert(priorty, id);
            GetSubsystem<ResourceCache>()->AddResourceDir(m_ModDescriptors[id].GetDirectory(), priorty);
            return true;
        }
    }
    return false;
}

bool ModManager::Deactivate(const Urho3D::String& id)
{
    if (m_ActiveMods.Contains(id))
    {
        m_ActiveMods.Remove(id);
        GetSubsystem<ResourceCache>()->RemoveResourceDir(m_ModDescriptors[id].GetDirectory());
        return true;
    }
    return false;
}

const Urho3D::HashMap<Urho3D::String, Mod>& ModManager::GetModDescriptors() const
{
    return m_ModDescriptors;
}

bool ModManager::IsActive(Urho3D::String id) const
{
    return m_ActiveMods.Contains(id);
}

void ModManager::ModActivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace ModActivated;
    String id = eventData[P_ID].ToString();
    unsigned int priority = eventData[P_PRIORITY].GetUInt();

    Activate(id, priority);
}

void ModManager::ModDeactivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace ModDeactivated;
    String id = eventData[P_ID].ToString();

    Deactivate(id);
}

void ModManager::ModOrderSaved(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    Save();
}
