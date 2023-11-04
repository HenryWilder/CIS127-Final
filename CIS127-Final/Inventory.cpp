#include "Inventory.hpp"

bool Inventory::Has(const string& key) const
{
    return items.contains(key);
}

int Inventory::Count(const string& key) const
{
    auto it = items.find(key);
    if (it != items.end())
    {
        return it->second;
    }
    return 0;
}

const map<string, int>& Inventory::GetAll() const
{
    return items;
}

bool Inventory::IsEmpty() const
{
    return items.empty();
}

void Inventory::Add(const string& key, int count)
{
    assert(count > 0);
    if (items.contains(key))
    {
        items.at(key) += count;
    }
    else
    {
        items.emplace(key, count);
    }
}

int Inventory::TryRemove(const string& key, int count)
{
    assert(count > 0);
    auto it = items.find(key);
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

bool Inventory::ForceRemove(const string& key, int count)
{
    assert(count > 0);
    auto it = items.find(key);
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

void Inventory::RemoveAll(const string& key)
{
    items.erase(key);
}

void Inventory::RemoveAll()
{
    items.clear();
}

void Inventory::Print() const
{
    cout << "Your current inventory:\n";
    for (const auto& item : items)
    {
        cout << "- " << item.second << " " << item.first << "\n";
    }
}

string Inventory::Prompt(const string& prompt) const
{
    cout << prompt;
    for (const auto& it : items)
    {
        cout << "\n- " << it.first;
    }
    cout << endl;
    while (true) // repeats until return
    {
        string input;
        cout << "> ";
        getline(cin, input);
        if (items.contains(input))
        {
            return input;
        }
    }
}

void Inventory::Init()
{
    RemoveAll();
    Add("gold", 5);
    Add("sword", 20);
}

void Inventory::Save(ostream& ofs) const
{
    ofs << "items: " << items.size();
    for (const auto& [name, num] : items)
    {
        ofs << "  " << name << " - " << num << '\n';
    }
}

void Inventory::Load(istream& ifs)
{
    items.clear();
    size_t numItems;
    ifs.ignore(16, ':') >> numItems;
    for (size_t i = 0; i < numItems; ++i)
    {
        string name;
        int num;
        (ifs >> name).ignore(3, '-') >> num;
        items.emplace(name, num);
    }
}
