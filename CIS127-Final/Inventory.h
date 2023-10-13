#pragma once
#include <string>
#include <map>
#include "Menu.h"
using std::string;
using std::map;

enum class Item
{
    EMPTY_BOTTLE,
    PROT_CLOAK, // Cloak of Protection makes you harder to hit

    // If both the Doom's Groom and Bride's Defense are equipped together, both become +2
    BETTER_SWORD, // +1 sword - "Doom's Groom"
    BETTER_SHIELD, // +1 shield - "Bride's Defense"

    WAND_MAGMISS, // A wand of magic missile that deals 3d4+3 damage without a check to hit but can only be used twice
    GLAM_ARMOR, // Glamoured Armor that allows you to blend in with human enemies and initiate dialog
};

string GetItemName(Item item);

using ItemMenu = Menu<Item>;

class Inventory
{
public:
    ItemMenu GenerateMenu() const;

    void AddItem(Item item, size_t qty = 1);
    // If trying to remove more than exist, none are removed and the function returns false.
    bool TryRemoveItem(Item item, size_t qty = 1);

private:
    map<Item, size_t> items;
};
