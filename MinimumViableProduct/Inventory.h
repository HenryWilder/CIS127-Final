#pragma once
#include "Utilities.h"

enum class Item
{
    UNKNOWN = 0,
    BASIC_SWORD,
    BASIC_SHIELD,
    BASIC_ARMOR,
    BASIC_POTION,
    GOLD_PIECES,
    DUST,
};

constexpr const char* itemNames[]
{
    "[UNKNOWN]",
    "Basic Sword",
    "Basic Shield",
    "Basic Armor",
    "Potion",
    "Gold",
    "Dust",
};

string ItemEnumToName(Item item);
Item ItemEnumFromName(const char* name);
inline Item ItemEnumFromName(const string& name) { return ItemEnumFromName(name.c_str()); }


struct ItemSlot
{
    ItemSlot() = default;
    ItemSlot(Item item);
    ItemSlot(Item item, int count);

    Item item;
    int count;
};

ostream& operator<<(ostream& stream, const ItemSlot& slot);
istream& operator>>(istream& stream, ItemSlot& slot);


struct Inventory
{
    Inventory() = default;
    Inventory(initializer_list<ItemSlot> items);

    bool Contains(Item checkFor) const;
    size_t IndexOf(Item item) const;

    void Add(Item item, _In_range_(> , 0) int count = 1);
    bool Use(Item item, _In_range_(> , 0) int count = 1);

    void DoInventory();

    vector<ItemSlot> items;
};

ostream& operator<<(ostream& stream, const Inventory& inventory);
istream& operator>>(istream& stream, Inventory& inventory);
