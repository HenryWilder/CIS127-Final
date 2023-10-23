#pragma once
#include "Utilities.h"

enum class Item
{
    UNKNOWN,
    BASIC_SWORD,
    BASIC_SHIELD,
    BASIC_ARMOR,
    BASIC_POTION,
    GOLD_PIECES,
};

// The information about an abstract instance of an item
struct ItemInfo
{
    ItemInfo() = default;

    constexpr ItemInfo(Item id, cstring_t shortName, cstring_t name, cstring_t description) :
        id(id), shortName(shortName), name(name), description(description) {}

    Item id;
    cstring_t shortName, name, description;
};

constexpr ItemInfo errItem = ItemInfo(Item::UNKNOWN, "undefined", "[UNKNOWN]", "Error");
constexpr ItemInfo itemInfo[]
{
    ItemInfo(Item::BASIC_SWORD,  "sword",  "Basic Sword",  "@todo"),
    ItemInfo(Item::BASIC_SHIELD, "shield", "Basic Shield", "@todo"),
    ItemInfo(Item::BASIC_ARMOR,  "armor",  "Basic Armor",  "@todo"),
    ItemInfo(Item::BASIC_POTION, "potion", "Potion",       "@todo"),
    ItemInfo(Item::GOLD_PIECES,  "gold",   "Gold",         "@todo"),
};

constexpr const ItemInfo GetItemInfo(Item id)
{
    for (const ItemInfo& info : itemInfo)
        if (info.id == id) return info;
    return errItem;
}

inline const ItemInfo GetItemInfo(string shortName)
{
    for (const ItemInfo& info : itemInfo)
        if (shortName == info.shortName) return info;
    return errItem;
}


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

    _Ret_maybenull_ ItemSlot* GetSlot(Item item);

    void Add(Item item, _In_range_(> , 0) int count = 1);
    bool TryRemove(Item item, _In_range_(> , 0) int count = 1);

    void DoInventory();

    vector<ItemSlot> items;
};

ostream& operator<<(ostream& stream, const Inventory& inventory);
ostream& operator<<(ostream& stream, Inventory& inventory);
istream& operator>>(istream& stream, Inventory& inventory);
