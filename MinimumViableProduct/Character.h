#pragma once
#include "Utilities.h"
#include "Inventory.h"

class Character
{
public:
    Character() = default;

    Character(const string& name) :
        inventory(), name(name) {}

    inline string GetDisplayName() const
    {
        return name;
    }

    Inventory inventory;

protected:
    string name;
};


class Player :
    public Character
{
public:
    Player() = default;

    Player(string name) :
        Character(name), position(0, 0) {}

    Player(string name, IVec2 position) :
        Character(name), position(position) {}

    inline IVec2 GetPosition() const
    {
        return position;
    }

    inline void SetPosition(IVec2 position)
    {
        this->position = position;
    }

    inline void Move(IVec2 offset)
    {
        position = position + offset;
    }

    // Friendship simplifies initialization when loading from file
    friend istream& operator>>(istream&, Player&);

private:
    IVec2 position;
};

ostream& operator<<(ostream& stream, const Player& player);
istream& operator>>(istream& stream, Player& player);
