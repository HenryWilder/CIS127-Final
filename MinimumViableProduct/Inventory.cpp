#include "Inventory.h"

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


ItemSlot::ItemSlot(Item item) :
    item(item), count(1) {}

ItemSlot::ItemSlot(Item item, int count) :
    item(item), count(count) {}


Inventory::Inventory(initializer_list<ItemSlot> items) :
    items(items) {}

bool Inventory::Contains(Item checkFor) const
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

size_t Inventory::IndexOf(Item item) const
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

void Inventory::Add(Item item, _In_range_(>,0) int count)
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

bool Inventory::Use(Item item, _In_range_(>,0) int count)
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

void Inventory::DoInventory()
{
    cout << *this << '\n';
}


ostream& operator<<(ostream& stream, const ItemSlot& slot)
{
    return stream << "  " << slot.count << ' ' << ItemEnumToName(slot.item);
}

ostream& operator<<(ostream& stream, const Inventory& inventory)
{
    stream << inventory.items.size() << " items:\n";
    for (const ItemSlot& slot : inventory.items)
    {
        stream << slot << '\n';
    }
    return stream;
}


istream& operator>>(istream& stream, ItemSlot& slot)
{
    stream >> slot.count;
    string name;
    stream.ignore(1, ' ');
    getline(stream, name);
    slot.item = ItemEnumFromName(name);
    return stream; 
}

istream& operator>>(istream& stream, Inventory& inventory)
{
    size_t size;
    stream >> size;
    inventory.items.reserve(size);
    stream.ignore(16, '\n');
    while (inventory.items.size() < inventory.items.capacity())
    {
        ItemSlot slot;
        stream >> slot;
        inventory.items.push_back(slot);
    }
    return stream;
}
