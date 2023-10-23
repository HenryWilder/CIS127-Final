#include "Character.h"

using DialogueTree = void(*)();


ostream& operator<<(ostream& stream, const Player& character)
{
    return stream << "name: " << character.GetName() << '\n' << character.inventory;
}

istream& operator>>(istream& stream, Player& character)
{
    stream.ignore(16, ' ');
    string name;
    getline(stream, name) >> character.inventory;
    character.SetName(name);
    return stream;
}


ostream& operator<<(ostream& stream, const BaseNPC* npc)
{
    return stream << npc->GetName() << '\n' << npc->inventory;
}

istream& operator>>(istream& stream, BaseNPC* npc)
{
    // Name is readonly
    return stream >> npc->inventory;
}


ostream& operator<<(ostream& stream, const vector<BaseNPC*>& npcs)
{
    stream << npcs.size();
    for (const BaseNPC* npc : npcs)
    {
        stream << npc;
    }
    return stream;
}

istream& operator>>(istream& stream, vector<BaseNPC*>& npcs)
{
    size_t numSavedNPCs;
    stream >> numSavedNPCs;

    for (size_t i = 0; i < numSavedNPCs; ++i)
    {
        string name;
        getline(stream, name);

        for (BaseNPC* npc : npcs)
        {
            if (npc->GetName() == name)
            {
                stream >> npc;
                break;
            }
        }
    }
    return stream;
}
