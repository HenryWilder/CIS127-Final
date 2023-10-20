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
    const string& GetName() const override;

    void Move(int horizontal, int vertical);

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

ostream& operator<<(ostream& stream, const vector<BaseNPC*>& npcs);
istream& operator>>(istream& stream, _Inout_ vector<BaseNPC*>& npcs);
