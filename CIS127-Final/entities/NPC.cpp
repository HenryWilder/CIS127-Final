#include "utilities/pch/utilities.hpp"
#include "NPC.hpp"
#include "npcs/Baker.hpp"
#include "npcs/Blacksmith.hpp"
#include "npcs/Wizard.hpp"
#include "npcs/Monster.hpp"

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
