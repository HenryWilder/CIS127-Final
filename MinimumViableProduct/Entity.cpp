#include "Entity.h"

istream& operator>>(istream& stream, Entity& entity)
{
    entity.FromStream(stream);
    return stream;
}

ostream& operator<<(ostream& stream, const Entity& entity)
{
    entity.ToStream(stream);
    return stream;
}

void BaseNPC::DoTrade(Player* player)
{
    // todo
}

void BakerNPC::DoInteraction(Player* player)
{
    // todo
}
