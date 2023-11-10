#include "Inventory.hpp"
#include "Prompt.hpp"

bool Inventory::Has(Item item) const
{
    return items.contains(item);
}

int Inventory::Count(Item item) const
{
    auto it = items.find(item);
    if (it != items.end())
    {
        return it->second;
    }
    return 0;
}

const map<Item, int>& Inventory::GetAll() const
{
    return items;
}

bool Inventory::IsEmpty() const
{
    return items.empty();
}

void Inventory::Add(Item item, int count)
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

int Inventory::TryRemove(Item item, int count)
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

bool Inventory::ForceRemove(Item item, int count)
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

void Inventory::RemoveAll(Item item)
{
    items.erase(item);
}

void Inventory::RemoveAll()
{
    items.clear();
}

void Inventory::Print(ostream& stream) const
{
    List(stream, items);
}

Item Inventory::Prompt(const string& prompt) const
{
    return PromptKey(prompt, items);
}

void Inventory::Init()
{
    RemoveAll();
    Add(Item::Gold, 5);
    Add(Item::Sword, 20);
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
