#pragma once
#include "Utilities.h"
#include "Inventory.h"

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
    inline const string& GetName() const override { return name; }
    inline void SetName(const string& name) { this->name = name; }

private:
    string name;
};

ostream& operator<<(ostream& stream, const Player& character);
istream& operator>>(istream& stream, Player& character);

using DialogueTree = void(*)();

class BaseNPC :
    public Character
{
public:
    virtual void Interact(Player& player) = 0;
};

ostream& operator<<(ostream& stream, const BaseNPC* npc);
istream& operator>>(istream& stream, BaseNPC* npc);

ostream& operator<<(ostream& stream, const vector<BaseNPC*>& npcs);
// Expects npcs to already exist; only modifies them
// todo: Consider using unordered_map instead of vector
istream& operator>>(istream& stream, vector<BaseNPC*>& npcs);
