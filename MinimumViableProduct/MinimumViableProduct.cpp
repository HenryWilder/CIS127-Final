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
#include <variant>
#include <algorithm>
using std::find;
using std::find_if;
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
using std::variant;
using std::exception;
using byte = unsigned char;

struct ColoredText
{
    string text;
    byte r, g, b;
};

ColoredText Colored(const string& text, _In_range_(0, 255) byte r, _In_range_(0, 255) byte g, _In_range_(0, 255) byte b)
{
    return { text, r, g, b };
}

ColoredText Colored(const string& text, _In_range_(0, 0xFFFFFF) int hex)
{
    return { text, (byte)(hex >> 020), (byte)(hex >> 010), (byte)(hex >> 000) };
}

ostream& operator<<(ostream& stream, const ColoredText& text)
{
    return stream << "\x1B[38;2;"
        << (unsigned int)text.r << ';'
        << (unsigned int)text.g << ';'
        << (unsigned int)text.b << 'm'
        << text.text << "\x1B[0m";
}

constexpr int SKYBLUE = 0x0080FF;
constexpr int RED     = 0xFF0000;
constexpr int GOLD    = 0xFFD700;

inline ColoredText Skyblue(const string& text) { return Colored(text, SKYBLUE); }
inline ColoredText Red    (const string& text) { return Colored(text, RED); }
inline ColoredText Gold   (const string& text) { return Colored(text, GOLD); }


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


class Character
{
public:
    virtual const string& GetName() const = 0;

    int x, y;
    Inventory inventory;
};

class Player :
    public Character
{
public:
    const string& GetName() const override
    {
        return name;
    }

    void Move(int horizontal, int vertical)
    {
        x += horizontal;
        y += vertical;
    }

    string name;
};

ostream& operator<<(ostream& stream, const Player& character)
{
    return stream << character.name << '\n' << character.inventory;
}

istream& operator>>(istream& stream, Player& character)
{
    return getline(stream, character.name) >> character.inventory;
}

using DialogueTree = void(*)();

class BaseNPC :
    public Character
{
public:
    virtual void Interact(Player& player) = 0;
};

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


struct SaveData
{
    SaveData() = default;

    SaveData(Player player) : // New game constructor
        player(player) {}

    Player player;

    void Save(const char* filename) const
    {
        cout << Skyblue("[Saving...]\n");
        ofstream ofs(filename, ofstream::trunc);
        if (ofs.is_open())
        {
            ofs << player;

            ofs.close();
            cout << Skyblue("[Save successful]\n");
            return;
        }
        cout << Red("[Error: Save failed]\n");
    }

    bool Load(const char* filename)
    {
        cout << Skyblue("[Loading...]\n");
        ifstream ifs(filename);
        if (ifs.is_open())
        {
            ifs >> player;

            ifs.close();
            cout << Skyblue("[Load successful]\n");
            return true;
        }
        cout << Red("[Error: Load failed]\n");
        return false;
    }
};


int main()
{
    SaveData data;
    const char* filename = "save.txt";

    if (!data.Load(filename)) // New game
    {
        data = SaveData();

        cout << "What is your name?\n";
        getline(cin, data.player.name);

        data.Save(filename); // Create save file
    }

    cout << "Your name is " << data.player.name << ".\n\n";
    cout << "Input \"" << Gold("save") << "\" at any time to save the game " << Colored("without closing", 0xFFFFFF) << ".\n";
    cout << "Input \"" << Gold("quit") << "\" at any time to save and close the game.\n";
    cout << "Closing the window without inputting \"" << Gold("quit") << "\" will NOT save your progress.\n";
    cout << "This game auto-saves occasionally. \"" << Skyblue("[Saving...]") << "\" will be shown when this happens.\n\n";

    while (true)
    {
        string input;
        cin >> input;

        if (input == "items")
        {
            cout << data.player.inventory;
        }
        if (input == "quit")
        {
            data.Save(filename);
            break;
        }
    }

    return 0;
}
