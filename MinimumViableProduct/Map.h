#pragma once
#include "Utilities.h"
#include "Character.h"

// Base class
class Entity
{
public:
    // The name of the category
    // This is what the user would type in to access the entity
    virtual string GetCategoryName() const = 0;

private:
    int x, y;
};

ostream& operator<<(ostream& stream, const Entity& map);
istream& operator>>(istream& stream, Entity& map);


class Map
{
public:
    void DoMovement(Player* player);

    bool IsValidTile(int x, int y) const;

    // Returns nullptr if there is no entity
    const Entity* GetEntityAtPosition(int x, int y) const;

private:
    vector<Entity> entities;
};

ostream& operator<<(ostream& stream, const Map& map);
istream& operator>>(istream& stream, Map& map);
