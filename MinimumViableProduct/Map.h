#pragma once
#include "Utilities.h"

class Player;
class Entity;

// Set of directions with names and offsets - used for providing movement options to the player
// @type Anonymous structure - easier to use thanks to C++17 structured bindings
constexpr struct { cstring_t name; IVec2 offset; } directions[] =
{
    { "west",  { -1,  0 } },
    { "east",  { +1,  0 } },
    { "north", {  0, -1 } },
    { "south", {  0, +1 } },
};

struct Tile
{
    bool isWall;
    Entity* entity; // The entity located at the tile
};

ostream& operator<<(ostream& stream, const Tile& tile);
istream& operator>>(istream& stream,       Tile& tile);

class Map
{
    void _GetMovementOptionsFromPosition(PromptOptionList& options, IVec2 position);

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

    // Simplifies loading saving/loading
    friend ostream& operator<<(ostream&, const Map&);
    friend istream& operator>>(istream&, Map&);

private:
    unsigned int seed;                // Allows predictable generation after loading the map from a file.
    unordered_map<IVec2, Tile> tiles; // Procedurally generated as player explores. Tiles in "fog of war" do not exist.
};

ostream& operator<<(ostream& stream, const Map& map);
istream& operator>>(istream& stream,       Map& map);
