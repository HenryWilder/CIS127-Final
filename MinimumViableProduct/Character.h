#pragma once
#include "Utilities.h"
#include "Inventory.h"

class Character
{
public:
    // Display name while playing
    virtual string GetDisplayName() const = 0;

    Inventory inventory;
};


class Player :
    public Character
{
public:
    Player() = default;

    Player(string name) :
        name(name), position(0, 0) {}

    Player(string name, IVec2 position) :
        name(name), position(position) {}

    inline string GetDisplayName()                   const override { return name; }
    inline void   SetDisplayName(const string& name)                { this->name = name; }

    inline IVec2 GetPosition()               const { return position; }
    inline void  SetPosition(IVec2 position)       { this->position = position; }
    inline void  Move       (IVec2 offset)         { position = position + offset; }

    // Friendship simplifies initialization when loading from file
    friend istream& operator>>(istream&, Player&);

private:
    string name;
    IVec2 position;
};

ostream& operator<<(ostream& stream, const Player& player);
istream& operator>>(istream& stream, Player& player);
