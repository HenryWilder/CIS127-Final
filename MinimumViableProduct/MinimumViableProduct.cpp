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

constexpr bool USE_COLORED_TEXT = true;

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
    if constexpr (USE_COLORED_TEXT)
    {
        return stream << "\x1B[38;2;"
            << (unsigned int)text.r << ';'
            << (unsigned int)text.g << ';'
            << (unsigned int)text.b << 'm'
            << text.text << "\x1B[0m";
    }
    else
    {
        return stream << text.text;
    }
}

constexpr int SKYBLUE = 0x0080FF;
constexpr int WHITE   = 0xFFFFFF;
constexpr int RED     = 0xFF0000;
constexpr int GREEN   = 0x00FF00;
constexpr int BLUE    = 0x0000FF;
constexpr int GOLD    = 0xFFD700;


string PromptString(const string& query)
{
    cout << query << "\n> ";
    string input;
    getline(cin, input);
    return input;
}

vector<string>::const_iterator Prompt(const string& query, const vector<string>& options)
{
    cout << query;
    for (const auto& option : options)
    {
        string optText = option;
        cout << "\n: " << Colored(optText, GOLD);
    }
    cout << '\n';

    auto it = options.end();
    while (it == options.end())
    {
        cout << "> ";
        string input;
        getline(cin, input);
        it = find(options.begin(), options.end(), input);
        if (it == options.end() && input == "quit")
        {
            return options.end();
        }
    }

    return it;
}


using ItemNum_t = unsigned int;
enum class Item : ItemNum_t
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

string ItemEnumToName(Item item)
{
    return itemNames[(size_t)item];
}

Item ItemEnumFromName(const char* name)
{
    constexpr size_t NUM_ITEMS = _countof(itemNames);

    for (size_t itemIndex = 1; itemIndex < NUM_ITEMS; ++itemIndex)
    {
        if (strcmp(name, itemNames[itemIndex]) == 0)
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
    return stream << slot.count << ' ' << ItemEnumToName(slot.item);
}

istream& operator>>(istream& stream, ItemSlot& slot)
{
    stream >> slot.count;
    string name;
    getline(stream, name);
    slot.item = ItemEnumFromName(name);
    return stream; 
}


struct Inventory
{
    Inventory() = default;

    Inventory(initializer_list<ItemSlot> items) :
        items(items) {}

    bool Contains(Item checkFor) const
    {
        for (const ItemSlot& item : items)
        {
            if (item.item == checkFor)
            {
                return true;
            }
        }
        return false;
    }

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

    void Add(Item item, _In_range_(>,0) int count = 1)
    {
        size_t index = IndexOf(item);
        if (index < items.size())
        {
            items[index].count += count;
        }
        else
        {
            items.push_back({ item, count });
        }
    }

    bool Use(Item item, _In_range_(>,0) int count = 1)
    {
        size_t index = IndexOf(item);
        if (index < items.size() && items[index].count <= count)
        {
            if (items[index].count == count)
            {
                items.erase(items.begin() + index);
            }
            else
            {
                items[index].count -= count;
            }
            return true;
        }
        return false;
    }

    vector<ItemSlot> items;
};

ostream& operator<<(ostream& stream, const Inventory& inventory)
{
    stream << inventory.items.size() << '\n';
    for (const ItemSlot& slot : inventory.items)
    {
        stream << slot << '\n';
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
        cout << Colored("[Saving...]", SKYBLUE);
        ofstream ofs(filename, ofstream::trunc);
        if (ofs.is_open())
        {
            ofs << player;

            ofs.close();
            cout << Colored("[Save successful]\n", SKYBLUE);
            return;
        }
        cout << Colored("[Save failed]\n", RED);
    }

    bool Load(const char* filename)
    {
        cout << Colored("[Loading...]", SKYBLUE);
        ifstream ifs(filename);
        if (ifs.is_open())
        {
            ifs >> player;

            ifs.close();
            cout << Colored("[Load successful]\n", SKYBLUE);
            return true;
        }
        cout << Colored("[Load failed]\n", RED);
        return false;
    }
};


int main()
{
    SaveData data;
    const char* filename = "save.txt";

    if (!data.Load(filename)) // New game
    {
        cout << Colored("STARTING NEW GAME\n", GOLD);

        data = SaveData();

        data.player.name = PromptString("What is your name?");

        data.Save(filename); // Create save file
    }

    cout
        << "Your name is " << data.player.name << ".\n"
        << '\n'
        << "Input \"" << Colored("save", GOLD) << "\" at any time to save the game " << Colored("without closing", WHITE) << ".\n"
        << "Input \"" << Colored("quit", GOLD) << "\" at any time to save and close the game.\n"
        << "Closing the window without inputting \"" << Colored("quit", GOLD) << "\" will NOT save your progress.\n"
        << "This game auto-saves occasionally. \"" << Colored("[Saving...]", SKYBLUE) << "\" will be shown when this happens.\n"
        << '\n';

    data.player.inventory.Add(Item::BASIC_SWORD);

    while (true)
    {
        vector<string> options = { "items" };
        auto it = Prompt("Action", options);

        if (it == options.end())
        {
            data.Save(filename);
            return 0;
        }

        if (*it == "items")
        {
            cout << data.player.inventory << '\n';
        }

        data.player.inventory.Add(Item::DUST);
    }

    return 0;
}
