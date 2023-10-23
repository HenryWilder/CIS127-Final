#pragma once
#include "Utilities.h"

class Player;
class Entity;

struct Tile
{
    bool isWall;
    Entity* entity; // The entity located at the tile
};

class Map
{
public:
    void DoMovement(Player* player);

    // @brief If position is in fog of war, a new tile is generated.
    // @returns true if the tile is floor (empty space), false if the tile is wall.
    Tile GetTile(IVec2 position);

    // Call at the end of the program to release all entities
    void Free();

private:
    size_t seed;                      // Allows predictable generation after loading the map from a file.
    unordered_map<IVec2, Tile> tiles; // Procedurally generated as player explores. Tiles in "fog of war" do not exist.
};

ostream& operator<<(ostream& stream, const Map& map);
istream& operator>>(istream& stream, Map& map);
