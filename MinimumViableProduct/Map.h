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
    void DoMovement(Player& player);

    // If position is in fog of war, a new tile is generated.
    Tile GetTile(IVec2 position);

private:
    unsigned int seed;                // Allows predictable generation after loading the map from a file.
    unordered_map<IVec2, Tile> tiles; // Procedurally generated as player explores. Tiles in "fog of war" do not exist.
};

ostream& operator<<(ostream& stream, const Map& map);
istream& operator>>(istream& stream, Map& map);
