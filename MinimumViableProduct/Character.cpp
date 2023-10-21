#include "Character.h"

const string& Player::GetName() const
{
    return name;
}

void Player::Move(int horizontal, int vertical)
{
    x += horizontal;
    y += vertical;
}

using DialogueTree = void(*)();


ostream& operator<<(ostream& stream, const Player& character)
{
    return stream << "name: " << character.name << '\n' << character.inventory;
}

istream& operator>>(istream& stream, Player& character)
{
    stream.ignore(16, ' ');
    return getline(stream, character.name) >> character.inventory;
}


ostream& operator<<(ostream& stream, const vector<BaseNPC*>& npcs)
{
    stream << npcs.size();
    for (const BaseNPC* npc : npcs)
    {
        stream << npc->GetName() << '\n' << npc->inventory;
    }
    return stream;
}

istream& operator>>(istream& stream, _Inout_ vector<BaseNPC*>& npcs)
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
                stream >> npc->inventory;
                break;
            }
        }
    }
    return stream;
}
