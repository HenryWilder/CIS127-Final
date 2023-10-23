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
    virtual void DoInteraction(Player* player) = 0;

    inline int GetX() const { return x; }
    inline int GetY() const { return y; }
    inline bool IsAtPosition(int x, int y) const { return this->x == x && this->y == y; }

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
    // Returns nullptr if there is no entity
    Entity* GetEntityAtPosition(int x, int y);

    // Returns entities.size() if no entity at supplied position
    size_t EntityIndexFromPosition(int x, int y) const;

    vector<Entity*> entities;
    vector<bool> tiles;
};

ostream& operator<<(ostream& stream, const Map& map);
istream& operator>>(istream& stream, Map& map);
