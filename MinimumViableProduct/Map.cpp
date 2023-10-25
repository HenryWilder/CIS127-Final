#include "Map.h"
#include "Entity.h"
#include "Character.h"

void Map::_PrintArea(IVec2 playerPos, int xMin, int yMin, int xMax, int yMax) const
{
    // See https://cplusplus.com/doc/ascii/

    constexpr char fogOfWar_ch = '\xB0';
    constexpr char wall_ch     = '\xDB';
    constexpr char free_ch     = ' ';
    constexpr char ent_ch      = '\x04';
    constexpr char npc_ch      = '\x01';
    constexpr char player_ch   = '\xA7';

    size_t height = (size_t)(yMax - yMin);
    size_t width  = (size_t)(xMax - xMin);
    size_t buffHeight = height + 1; // +1 to convert max index to count
    size_t buffWidth  = width  + 2; // +1 extra for newline
    size_t buffSize = buffHeight * buffWidth;

    string buff;
    buff.reserve(buffSize);

    auto BuffIndex = [=](IVec2 pos)
        {
            size_t xRel = (size_t)((long long)pos.x - xMin);
            size_t yRel = (size_t)((long long)pos.y - yMin);
            size_t index = yRel * buffWidth + xRel;
            return index;
        };

    for (int y = yMin; y <= xMax; ++y)
    {
        for (int x = xMin; x <= xMax; ++x)
        {
            bool isWall = tiles.at(TileIndex(x, y)) == Tile::WALL;
            buff.push_back(isWall ? wall_ch : free_ch);
        }
        buff.push_back('\n');
    }

    vector<Entity*> entitiesInPrintArea;
    GetEntitiesInArea(entitiesInPrintArea, xMin, yMin, xMax, yMax);
    for (Entity* entity : entitiesInPrintArea)
    {
        bool isNPC = entity->IsNPC();
        buff.at(BuffIndex(entity->GetPosition())) = isNPC ? npc_ch : ent_ch;
    }

    buff.at(BuffIndex(playerPos)) = player_ch;

    cout << buff << std::endl;
}

void Map::_PrintDebug(IVec2 playerPos) const
{
#if _DEBUG
    _PrintArea(playerPos, -MAP_EXTENT, -MAP_EXTENT, MAP_EXTENT, MAP_EXTENT);
#endif
}

void Map::PrintArea(IVec2 playerPos, int extent) const
{
    // horizontally 2x as wide as vertically
    _PrintArea(playerPos,
        playerPos.x - extent * 2,
        playerPos.y - extent,
        playerPos.x + extent * 2,
        playerPos.y + extent);
}

void Map::Generate()
{
    srand(GetSeedHash());

    vector<double> buffer(MAP_AREA, 0.0);

    // Walls
    for (int resolution = 1; resolution <= MAP_WIDTH; resolution <<= 1)
    {
        int step = MAP_WIDTH / resolution;
        for (int y = -MAP_EXTENT; y <= MAP_EXTENT; y += step)
        {
            for (int x = -MAP_EXTENT; x <= MAP_EXTENT; x += step)
            {
                int yEnd = y + step;
                int xEnd = x + step;
                double value = (double)rand() / RAND_MAX;
                for (int y1 = y; y1 < yEnd; ++y1)
                {
                    for (int x1 = x; x1 < xEnd; ++x1)
                    {
                        size_t index = TileIndex(x1, y1);
                        buffer.at(index) += value;
                    }
                }
            }
        }
    }

    tiles.clear(); // in case
    tiles.reserve(MAP_AREA);

    // Convert double to bool
    for (double value : buffer)
    {
        tiles.push_back(value < 0.2 ? Tile::WALL : Tile::FREE);
    }

    //entities.clear();
    //entities.reserve(NUM_ENTITIES);
    //
    //// Entities
    //for (size_t i = 0; i < NUM_ENTITIES; ++i)
    //{
    //    entities.push_back(NewEntityOfType(EntityType::ENT_CHEST));
    //    entities.back()->SetPosition(rand() % MAP_WIDTH - MAP_EXTENT, rand() % MAP_WIDTH - MAP_EXTENT);
    //}
}

size_t Map::TileIndex(int x, int y) const
{
    long long yRel = (long long)y + MAP_EXTENT;
    long long xRel = (long long)x + MAP_EXTENT;
    size_t index = yRel * MAP_WIDTH + xRel;
    return index;
}

void Map::GetEntitiesInArea(_Inout_ vector<Entity*>& inArea, int xMin, int yMin, int xMax, int yMax) const
{
    for (Entity* entity : entities)
    {
        assert(entity != nullptr, "Map should not contain null entities.");

        auto [x, y] = entity->GetPosition();

        if (xMin <= x && x <= xMax &&
            yMin <= y && y <= yMax)
        {
            inArea.push_back(entity);
        }
    }
}

_Ret_maybenull_ Entity* Map::GetEntityAt(_In_ const vector<Entity*>& from, IVec2 position)
{
    for (Entity* entity : from)
    {
        assert(entity != nullptr, "Map should not contain null entities.");

        if (entity->GetPosition() == position)
        {
            return entity;
        }
    }
    return nullptr;
}

void Map::DoMovement(Player& player)
{
    // Set of directions with names and offsets - used for providing movement options to the player
    // @type Anonymous structure - easier to use thanks to C++17 structured bindings
    constexpr struct { const char* name; IVec2 offset; } directions[] =
    {
        { "north",     {  0, -1 } },
        { "northeast", { +1, -1 } },
        { "east",      { +1,  0 } },
        { "southeast", { +1, +1 } },
        { "south",     {  0, +1 } },
        { "southwest", { -1, +1 } },
        { "west",      { -1,  0 } },
        { "northwest", { -1, -1 } },
    };

    PromptOptionList options;

    IVec2 playerPos = player.GetPosition();

    vector<Entity*> entitiesNearPlayer;
    GetEntitiesInArea(entitiesNearPlayer,
        playerPos.x - 1,
        playerPos.y - 1,
        playerPos.x + 1,
        playerPos.y + 1);

    // Populate options list
    for (size_t i = 0; i < _countof(directions); ++i)
    {
        const auto& [directionName, offset] = directions[i];

        IVec2 posPrime = playerPos + offset;

        Tile tile = GetTile(posPrime);

        if (tile == Tile::WALL)
        {
            continue;
        }

        if (posPrime.x != 0 && posPrime.y != 0)
        {
            IVec2 xPosPrime = IVec2(posPrime.x, playerPos.y);
            IVec2 yPosPrime = IVec2(playerPos.x, posPrime.y);
            if (GetTile(xPosPrime) == Tile::WALL &&
                GetTile(yPosPrime) == Tile::WALL)
            {
                continue;
            }
        }

        string input;
        string description;
        Entity* tileEntity = GetEntityAt(entitiesNearPlayer, posPrime);

        if (tileEntity) // Implied "!= nullptr" because nullptr is 0
        {
            cstring_t typeName = tileEntity->GetTypeName();
            input       = format("{} {}", directionName, typeName);
            description = format("Interact with the {} to your {}", typeName, directionName);
        }
        else
        {
            input       = directionName;
            description = format("Move one tile {}", directionName);
        }

        options.emplace_back(input, description);
    }

#if 0
    _PrintDebug(player.GetPosition());
#else
    PrintArea(player.GetPosition(), 3);
#endif

    // Handle input
    {
        auto it = Prompt("What would you like to do?", options);
        string selectedName  = it->input;
        size_t selectedIndex = it - options.begin();

        IVec2 offset{};
        bool found = false;
        string selectionDirectionName = selectedName.substr(0, selectedName.find(' '));
        for (const auto& [directionName, directionOffset] : directions)
        {
            if (selectionDirectionName == directionName)
            {
                offset = directionOffset;
                found = true;
                break;
            }
        }
        assert(found, "Direction unknown");

        // Explicit movement - never has a space
        if (selectedName.find(' ') == string::npos)
        {
            player.Move(offset);
            return;
        }

        IVec2 posPrime = player.GetPosition() + offset;
        Entity* entity = GetEntityAt(posPrime);
        assert(!!entity, "There must be an entity if there is a space in " + selectedName);

        // Entity interaction
        entity->DoInteraction(player);
    }
}

double NoiseValue(unsigned int seed, IVec2 pos)
{
    srand(seed);
    // todo
    return 0.0;
}

bool Chance(int n, int in_every)
{
    return rand() % in_every < n;
}

Tile Map::GetTile(IVec2 position)
{
    return tiles.at(TileIndex(position));
}

ostream& operator<<(ostream& stream, const Map& map)
{
    stream << map.seed << '\n' << map.entities.size();
    for (Entity* ent : map.entities)
    {
        stream << '\n' << ent;
    }
    return stream;
}

istream& operator>>(istream& stream, Map& map)
{
    string seed;
    size_t numEntities;
    getline(stream, seed) >> numEntities;
    map = Map(seed);
    map.entities.reserve(numEntities);
    for (size_t i = 0; i < numEntities; ++i)
    {
        Entity* entity;
        stream >> entity;
        map.entities.push_back(entity);
    }
    return stream;
}
