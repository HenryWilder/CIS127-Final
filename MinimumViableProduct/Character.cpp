#include "Character.h"

using DialogueTree = void(*)();


ostream& operator<<(ostream& stream, const Player& player)
{
    return stream << player.GetName() << '\n' << player.GetPosition() << '\n' << player.inventory;
}

istream& operator>>(istream& stream, Player& player)
{
    return getline(stream, player.name) >> player.position >> player.inventory;
}
