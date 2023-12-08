#include "NPC.hpp"

NPC *NewNPCOfType(NPCType type, Collective collective)
{
    switch (type)
    {
        case NPCType::Baker:      return new Baker     (collective);
        case NPCType::Blacksmith: return new Blacksmith(collective);
        case NPCType::Wizard:     return new Wizard    (collective);
        case NPCType::Monster:    return new Monster   ( ); // Always "monsters"
    }
    throw new NotImplementedException(npcTypes.KeyAt(type));
}
