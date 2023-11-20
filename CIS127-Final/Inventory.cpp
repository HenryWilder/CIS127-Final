#include "utilities.hpp"
#include "Inventory.hpp"
#include "Prompt.hpp"

bool Inventory::HasItem(Item item) const
{
    return items.contains(item);
}

int Inventory::CountItem(Item item) const
{
    auto it = items.find(item);
    if (it != items.end())
    {
        return it->second;
    }
    return 0;
}

void Inventory::AddItem(Item item, _In_range_(>, 0) int count)
{
    assert(count > 0);
    if (items.contains(item))
    {
        items.at(item) += count;
    }
    else
    {
        items.emplace(item, count);
    }
}

int Inventory::TryRemoveItemQty(Item item, _In_range_(>, 0) int count)
{
    assert(count > 0);
    auto it = items.find(item);
    if (it != items.end())
    {
        if (it->second == count)
        {
            items.erase(it);
            return 0;
        }
        else if (it->second > count)
        {
            return it->second -= count;
        }
    }
    return -1;
}

bool Inventory::ForceRemoveItemQty(Item item, _In_range_(>, 0) int count)
{
    assert(count > 0);
    auto it = items.find(item);
    if (it != items.end())
    {
        if (it->second <= count)
        {
            items.erase(it);
            return it->second == count;
        }
        else // it->second > count
        {
            it->second -= count;
            return true;
        }
    }
    return false;
}

void Inventory::PrintInventory(ostream& stream) const
{
    List(stream, items);
}

Item Inventory::PromptItem(const string& prompt) const
{
    return PromptKey(prompt, items);
}

void Inventory::Init()
{
    ClearInventory();
    AddItem(Item::Gold, 5);
    AddItem(Item::Sword, 20);
}

void Inventory::Save(ostream& ofs) const
{
    ofs << "items: " << items.size() << '\n';
    for (const auto& [item, qty] : items)
    {
        ofs << "  " << item << " - " << qty << '\n';
    }
}

void Inventory::Load(istream& ifs)
{
    items.clear();
    size_t numItems;
    ifs.ignore(16, ':') >> numItems;
    for (size_t i = 0; i < numItems; ++i)
    {
        Item item;
        int qty;
        (ifs >> item).ignore(3, '-') >> qty;
        items.emplace(item, qty);
    }
}
