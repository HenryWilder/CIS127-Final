#pragma once
#include "Utilities.h"

class Player;
class Entity;

enum class Tile : bool
{
    FREE = false,
    WALL = true,
};

class Map
{
    static constexpr int MAP_WIDTH = 64;
    static constexpr int MAP_EXTENT = MAP_WIDTH / 2;
    static constexpr int MAP_AREA = MAP_WIDTH * MAP_WIDTH;

    // Randomly generates the map
    void Generate(const string& seed);

    size_t TileIndex(int x, int y) const;
    inline size_t TileIndex(IVec2 p) const
    {
        return TileIndex(p.x, p.y);
    }

    void GetEntitiesInArea(_Inout_ vector<Entity*>& inArea, int xMin, int yMin, int xMax, int yMax) const;
    static _Ret_maybenull_ Entity* GetEntityAt(_In_ const vector<Entity*>& from, IVec2 position);
    inline _Ret_maybenull_ Entity* GetEntityAt(IVec2 position) const
    {
        return GetEntityAt(entities, position);
    }

public:
    Map() = default;

    Map(const string& seed) :
        seed(seed) { Generate(seed); }

    void DoMovement(Player& player);

    // If position is in fog of war, a new tile is generated.
    Tile GetTile(IVec2 position);

    inline string GetSeed() const
    {
        return seed;
    }
    inline unsigned int GetSeedHash() const
    {
        return (unsigned int)hash<string>()(GetSeed());
    }

    void _PrintArea(IVec2 playerPos, int xMin, int yMin, int xMax, int yMax) const;
    void _PrintDebug(IVec2 playerPos) const;
    void PrintArea(IVec2 playerPos, int extent) const;

    // Simplifies loading saving/loading
    friend ostream& operator<<(ostream&, const Map&);
    friend istream& operator>>(istream&, Map&);

private:
    // Instead of storing the entire map to the file, only the seed and entities are stored.
    // The seed allows the map to be regenerated identically without storing the map on the disk.
    string seed;
    vector<Tile> tiles;
    vector<Entity*> entities;
};

ostream& operator<<(ostream& stream, const Map& map);
istream& operator>>(istream& stream,       Map& map);
