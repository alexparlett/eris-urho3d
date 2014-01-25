////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Locale.h"
#include "Settings.h"

#include <ResourceCache.h>
#include <XMLFile.h>
#include <Log.h>

using namespace Urho3D;

Page::Page(Urho3D::XMLElement& page)
{
    for (XMLElement line = page.GetChild("line"); line; line = line.GetNext("line"))
    {
        int id = line.GetInt("id");
        String value = line.GetValue();

        if (id > 0 && !value.Empty())
            lines_[id] = value;
    }
}

Urho3D::String Page::GetLine(int line) const
{
    HashMap<int, String>::ConstIterator findIt = lines_.Find(line);
    return findIt != lines_.End() ? findIt->second_ : String::EMPTY;
}

Locale::Locale(Urho3D::Context* context) :
    Object(context)
{
}

void Locale::Load(const Urho3D::String& fileName)
{
    pages_.Clear();

    XMLFile* file = GetSubsystem<ResourceCache>()->GetResource<XMLFile>("Locales/" + fileName + ".xml");
    if (file)
    {
        XMLElement root = file->GetRoot();
        for (XMLElement page = root.GetChild("page"); page; page = page.GetNext("Child"))
        {
            int id = page.GetInt("id");

            if (id > 0)
                pages_[id] = SharedPtr<Page>(new Page(page));
        }
    }
}

Urho3D::String Locale::Localize(int page, int line) const
{
    HashMap<int, SharedPtr<Page>>::ConstIterator findIt = pages_.Find(page);
    if (findIt != pages_.End())
        return findIt->second_->GetLine(line);

    return String::EMPTY;
}

void Locale::Replace(Urho3D::String& line, const Urho3D::String& value) const
{
    line.Replace("{0}", value);
}

void Locale::Replace(Urho3D::String& line, const Urho3D::PODVector<Urho3D::String>& values) const
{
    for(unsigned i = 0; i < values.Size(); i++)
        line.Replace(ToString("{%d}",i), values[i]);
}