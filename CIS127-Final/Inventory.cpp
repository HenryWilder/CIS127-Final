#include "Inventory.h"

string GetItemName(Item item)
{
    switch (item)
    {
    case Item::EMPTY_BOTTLE:  return "Empty bottle";
    case Item::PROT_CLOAK:    return "Cloak of Protection";
    case Item::BETTER_SWORD:  return "Doom's Groom";
    case Item::BETTER_SHIELD: return "Bride's Defense";
    case Item::WAND_MAGMISS: return "Wand of Magic Missile";
    default: return "[UNKNOWN]";
    }
}

ItemMenu Inventory::GenerateMenu() const
{    
    ItemMenu menu;
    size_t i = 0;
    for (auto& [item, qty] : items)
    {
        string userInput = std::to_string(i);
        string friendlyName = GetItemName(item);
        Item token = item;
        menu.options.push_back({ userInput, friendlyName, token });
        ++i;
    }
    return menu;
}

void Inventory::AddItem(Item item, size_t qtyToAdd)
{
    if (qtyToAdd == 0)
    {
        return;
    }

    auto slot = items.find(item);
    if (slot != items.end())
    {
        size_t& qtyHeld = slot->second;
        qtyHeld += qtyToAdd;
    }
    else
    {
        items.insert({ item, qtyToAdd });
    }
}

bool Inventory::TryRemoveItem(Item item, size_t qtyToRemove)
{
    // I'm not sure whether removing 0 instances should be considered a successful move or not.
    if (qtyToRemove == 0)
    {
        return false;
    }

    auto slot = items.find(item);
    if (slot != items.end())
    {
        size_t& qtyHeld = slot->second;
        if (qtyToRemove <= qtyHeld)
        {
            if (qtyToRemove < qtyHeld)
            {
                qtyHeld -= qtyToRemove;
            }
            else if (qtyToRemove == qtyHeld)
            {
                items.erase(slot);
            }
            return true;
        }
    }
    return false;
}
