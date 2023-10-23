#pragma once
#include "Utilities.h"
#include "Inventory.h"

class Character
{
public:
    virtual string GetName() const = 0;

    Inventory inventory;
};


class Player :
    public Character
{
public:
    inline string GetName() const override
    {
        return name;
    }
    inline void SetName(const string& name)
    {
        this->name = name;
    }

    inline IVec2 GetPosition() const
    {
        return position;
    }
    inline void Move(IVec2 offset)
    {
        position = position + offset;
    }

    friend istream& operator>>(istream&, Player&);

private:
    string name;
    IVec2 position;
};

ostream& operator<<(ostream& stream, const Player& player);
istream& operator>>(istream& stream, Player& player);
