/**
* CIS 127 Final | Minimum Viable Product (in case I get too in-depth with the full version)
* @author Henry Lachman
* @description
* Option 2 - Dungeon Crawl Text Game
* - Store game information to a file
*   - File must contain at least 6 pieces of information
* - Prompt the user for directions
* - Gather and store items
* - Remove items from inventory when used
* - Play must encounter other "characters"
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <concepts>
using std::cin;
using std::cout;
using std::getline;
using std::string;
using std::vector;
using std::unordered_map;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::same_as;
using std::initializer_list;
using std::pair;


using ItemNum_t = unsigned int;
enum class Item : ItemNum_t
{
    UNKNOWN = 0,
    BASIC_SWORD,
    BASIC_SHIELD,
    BASIC_ARMOR,
    BASIC_POTION,
    GOLD,
};

constexpr const char* itemNames[]
{
    "UNKNOWN",
    "Basic Sword",
    "Basic Shield",
    "Basic Armor",
    "Basic Potion",
    "Gold",
};

string ItemEnumToName(Item item)
{
    return itemNames[(size_t)item];
}

Item ItemEnumFromName(const char* name)
{
    constexpr size_t NUM_ITEMS = _countof(itemNames);

    for (size_t itemIndex = 1; itemIndex < NUM_ITEMS; ++itemIndex)
    {
        if (strcmp(name, itemNames[itemIndex]))
        {
            return (Item)itemIndex;
        }
    }
    return Item::UNKNOWN;
}

Item ItemEnumFromName(const string& name)
{
    return ItemEnumFromName(name.c_str());
}


struct ItemSlot
{
    ItemSlot() = default;

    ItemSlot(Item item) :
        item(item), count(1) {}

    ItemSlot(Item item, int count) :
        item(item), count(count) {}

    Item item;
    int count;
};

ostream& operator<<(ostream& stream, const ItemSlot& slot)
{
    return stream << (ItemNum_t)slot.item << ' ' << slot.count;
}

istream& operator>>(istream& stream, ItemSlot& slot)
{
    return stream >> (ItemNum_t&)slot.item >> slot.count;
}


struct Inventory
{
    Inventory() = default;

    Inventory(initializer_list<ItemSlot> items) :
        items(items) {}

    size_t IndexOf(Item item) const
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            if (items[i].item == item)
            {
                return i;
            }
        }
        return items.size();
    }

    const ItemSlot& operator[](Item item)          const { return items[IndexOf(item)]; }
    const ItemSlot& operator[](const char* name)   const { return (*this)[ItemEnumFromName(name)]; }
    const ItemSlot& operator[](const string& name) const { return (*this)[name.c_str()]; }

    ItemSlot& operator[](Item item)          { return items[IndexOf(item)]; }
    ItemSlot& operator[](const char* name)   { return (*this)[ItemEnumFromName(name)]; }
    ItemSlot& operator[](const string& name) { return (*this)[name.c_str()]; }

    vector<ItemSlot> items;
};

ostream& operator<<(ostream& stream, const Inventory& inventory)
{
    stream << inventory.items.size() << '\n';
    for (const ItemSlot& slot : inventory.items)
    {
        stream << slot << '  ';
    }
    return stream;
}

istream& operator>>(istream& stream, Inventory& inventory)
{
    size_t size;
    stream >> size;
    inventory.items.reserve(size);
    while (inventory.items.size() < inventory.items.capacity())
    {
        ItemSlot slot;
        stream >> slot;
        inventory.items.push_back(slot);
    }
    return stream;
}


struct Character
{
    Character() = default;

    Character(string name) :
        name(name), inventory() {}

    Character(string name, initializer_list<ItemSlot> inventory) :
        name(name), inventory(inventory) {}

    string name;
    Inventory inventory;
};
using Player = Character;

ostream& operator<<(ostream& stream, const Character& character)
{
    return stream << character.name << '\n' << character.inventory;
}

istream& operator>>(istream& stream, Character& character)
{
    return getline(stream, character.name) >> character.inventory;
}


struct SaveData
{
    SaveData() = default;

    SaveData(Player player) : // New game constructor
        player(player), roomIndex(0) {}

    Player player;
    size_t roomIndex = 0;

    void Save(const char* filename) const
    {
        ofstream ofs(filename, ofstream::trunc);
        if (ofs.is_open())
        {
            ofs << player;

            ofs.close();
        }
    }

    bool Load(const char* filename)
    {
        ifstream ifs(filename);
        if (ifs.is_open())
        {
            ifs >> player;

            ifs.close();
            return true;
        }
        return false;
    }
};


int main()
{
    SaveData data;
    const char* filename = "save.txt";

    if (!data.Load(filename)) // New game
    {
        cout << "What is your name?\n";
        string name;
        getline(cin, name);

        data = SaveData(Player(name, {
            Item::BASIC_SWORD,
        }));

        data.Save(filename); // Create save file
    }

    cout << "Your name is " << data.player.name << ".\n";

    return 0;
}
