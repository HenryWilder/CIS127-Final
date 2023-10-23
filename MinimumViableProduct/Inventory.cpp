#include "Inventory.h"


ItemSlot::ItemSlot(Item item) :
    item(item), count(1) {}

ItemSlot::ItemSlot(Item item, int count) :
    item(item), count(count) {}


Inventory::Inventory(initializer_list<ItemSlot> items) :
    items(items) {}

_Ret_maybenull_ ItemSlot* Inventory::GetSlot(Item item)
{
    for (ItemSlot& slot : items)
        if (slot.item == item) return &slot;
    return nullptr;
}

void Inventory::Add(Item item, _In_range_(>,0) int count)
{
    ItemSlot* slot = GetSlot(item);
    if (slot)
    {
        slot->count += count;
    }
    else
    {
        items.push_back({ item, count });
    }
}

bool Inventory::TryRemove(Item item, _In_range_(>,0) int count)
{
    ItemSlot* slot = GetSlot(item);
    if (slot && slot->count <= count)
    {
        slot->count -= count;
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
    return stream << "  " << slot.count << " " << GetItemInfo(slot.item).shortName;
}

bool IsEmptySlot   (const ItemSlot& slot) { return slot.count == 0; }
bool IsNonEmptySlot(const ItemSlot& slot) { return slot.count != 0; }

ostream& operator<<(ostream& stream, const Inventory& inventory)
{
    size_t numNotZero = count_if(inventory.items, IsNonEmptySlot);

    stream << numNotZero << " items:\n";
    for (const ItemSlot& slot : inventory.items)
    {
        if (!IsEmptySlot(slot))
        {
            stream << slot << '\n';
        }
    }

    return stream;
}

ostream& operator<<(ostream& stream, Inventory& inventory)
{
    erase_if(inventory.items, IsEmptySlot);

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
    slot.item = GetItemInfo(name).id;
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
