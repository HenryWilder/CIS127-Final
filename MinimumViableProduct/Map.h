#pragma once
#include "Utilities.h"

class Player;
class Entity;

struct Tile
{
    bool isWall;
    Entity* entity; // The entity located at the tile
};

ostream& operator<<(ostream& stream, const Tile& tile);
istream& operator>>(istream& stream,       Tile& tile);

class Map
{
public:
    Map() = default;

    Map(unsigned int seed) :
        seed(seed) {}

    Map(string seed) :
        seed((unsigned int)hash<string>()(seed)) {}

    void DoMovement(Player& player);

    // If position is in fog of war, a new tile is generated.
    Tile GetTile(IVec2 position);

    inline unsigned int GetSeed() const { return seed; }

    void _PrintDebug(IVec2 playerPos) const;

    // Simplifies loading saving/loading
    friend ostream& operator<<(ostream&, const Map&);
    friend istream& operator>>(istream&, Map&);


private:
    unsigned int seed;                // Allows predictable generation after loading the map from a file.
    unordered_map<IVec2, Tile> tiles; // Procedurally generated as player explores. Tiles in "fog of war" do not exist.
};

ostream& operator<<(ostream& stream, const Map& map);
istream& operator>>(istream& stream,       Map& map);
