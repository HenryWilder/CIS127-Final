#include "Map.h"
#include "Entity.h"
#include "Character.h"

#if _DEBUG
void Map::_PrintDebug(IVec2 playerPos) const
{
    // See https://cplusplus.com/doc/ascii/

    constexpr char fogOfWar_ch = '\xB0';
    constexpr char wall_ch     = '\xDB';
    constexpr char free_ch     = ' ';
    constexpr char ent_ch      = '\x04';
    constexpr char npc_ch      = '\x01';
    constexpr char player_ch   = '\xA7';

    long xMin = LONG_MAX;
    long yMin = LONG_MAX;
    long xMax = LONG_MIN;
    long yMax = LONG_MIN;
    for (auto& [pos, _] : tiles)
    {
        xMin = min(xMin, (long)pos.x);
        yMin = min(yMin, (long)pos.y);
        xMax = max(xMax, (long)pos.x);
        yMax = max(yMax, (long)pos.y);
    }
    size_t height = (size_t)(yMax - yMin);
    size_t width  = (size_t)(xMax - xMin);
    size_t buffHeight = height + 1; // +1 to convert max index to count
    size_t buffWidth  = width  + 2; // +1 extra for newline
    size_t buffSize = buffHeight * buffWidth;
    string buff = string(buffSize, fogOfWar_ch);

    // Index of pos in the buffer
    auto index = [&](IVec2 pos)
    {
        size_t x = (size_t)((long)pos.x - xMin);
        size_t y = (size_t)((long)pos.y - yMin);
        assert(0 <= x && x <= width,  format("{} is outside of the range [0..{}]", x, width));
        assert(0 <= y && y <= height, format("{} is outside of the range [0..{}]", y, height));
        return y * buffWidth + x;
    };

    // Add a newline char to the end of each line.
    for (size_t line = 1; line <= buffHeight; ++line)
    {
        buff.at(line * buffWidth - 1) = '\n';
    }

    for (auto& [pos, tile] : tiles)
    {
        char tileChar;
        if (tile.isWall)
        {
            tileChar = wall_ch;
        }
        else if (tile.entity)
        {
            bool isNPC = (int)tile.entity->GetType() & (int)EntityType::_NPC;
            if (isNPC) // is npc
            {
                tileChar = npc_ch;
            }
            else
            {
                tileChar = ent_ch;
            }
        }
        else
        {
            tileChar = free_ch;
        }

        buff.at(index(pos)) = tileChar;
    }

    buff.at(index(playerPos)) = player_ch;

    cout << buff << std::endl;
}
#else
void Map::_PrintDebug() const {}
#endif

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

    // Populate options list
    for (size_t i = 0; i < _countof(directions); ++i)
    {
        const auto& [directionName, offset] = directions[i];

        IVec2 posPrime = player.GetPosition() + offset;

        Tile tile = GetTile(posPrime);

        if (tile.isWall)
        {
            continue;
        }

        string input;
        string description;
        if (Entity* entity = tile.entity) // Implied "!= nullptr" because nullptr is 0
        {
            cstring_t typeName = entity->GetTypeName();
            input       = format("{} {}", directionName, typeName);
            description = format("Interact with the {} {} of you", typeName, directionName);
        }
        else
        {
            input       = directionName;
            description = format("Move one space to the {}", directionName);
        }
        options.emplace_back(input, description);
    }

    _PrintDebug(player.GetPosition());

    // Handle input
    {
        auto it = Prompt("Where", options);
        string selectedName  = it->input;
        size_t selectedIndex = it - options.begin();

        IVec2 offset;
        for (const auto& [directionName, directionOffset] : directions)
        {
            if (selectedName.starts_with(directionName))
            {
                offset = directionOffset;
            }
        }

        // Explicit movement - never has a space
        if (selectedName.find(' ') == string::npos)
        {
            player.Move(offset);
            return;
        }

        Entity* entity = GetTile(player.GetPosition() + offset).entity;
        assert(!!entity, "There must be an entity if there is a space in " + selectedName);

        // Entity interaction
        entity->DoInteraction(player);
    }
}

// Always generates the same value for the same seed and position
double NoiseValue(unsigned int seed, IVec2 position)
{
    constexpr double TAU = 3.1415926535897932384 * 2.0;
    
    double value = 0.0;

    srand(seed);

    for (int octave = 1; octave <= 8; ++octave)
    {
        double frequency = 8.0 / octave;
        double offset = TAU * ((double)rand() / (double)RAND_MAX);
        value += cos(frequency * position.x + offset) / (double)octave;
    }

    for (int octave = 1; octave <= 8; ++octave)
    {
        double frequency = 8.0 / octave;
        double offset = TAU * ((double)rand() / (double)RAND_MAX);
        value += sin(frequency * position.y + offset) / (double)octave;
    }

    return value;
}

// todo: Improve map generation to make more traversable maps
Tile Map::GetTile(IVec2 position)
{
    if (auto it = tiles.find(position); it != tiles.end())
    {
        return it->second;
    }
    else // Generate new tile
    {
        constexpr int IS_WALL_MASK    = 0b1100000000;
        constexpr int HAS_ENTITY_MASK = 0b0011100000;
        constexpr int ENTITY_MASK     = 0b0000011111;
        constexpr int NOISE_MAX       = 0b1111111111;

        bool isWall;
        Entity* tileEntity = nullptr;

        int tileRand = (int)(NoiseValue(seed, position) * RAND_MAX) % RAND_MAX;

        isWall = (tileRand & IS_WALL_MASK) == IS_WALL_MASK;
        if (!isWall)
        {
            bool isEntityAtTile = (tileRand & HAS_ENTITY_MASK) == HAS_ENTITY_MASK;
            if (isEntityAtTile)
            {
                tileEntity = NewEntityOfType((EntityType)(tileRand % (int)EntityType::_NUM));
            }
        }
        
        return tiles.emplace(position, Tile(isWall, tileEntity)).first->second;
    }
}

ostream& operator<<(ostream& stream, const Tile& tile)
{
    return stream << (tile.isWall ? "wall" : "free") << ' ' << tile.entity;
}

istream& operator>>(istream& stream, Tile& tile)
{
    string isWallStr;
    stream >> isWallStr >> tile.entity;
    tile.isWall = isWallStr == "wall";
    assert(isWallStr == "wall" || isWallStr == "free", "Expected \"wall\" or \"free\"; got \"" + isWallStr + "\".");
    return stream;
}

ostream& operator<<(ostream& stream, const Map& map)
{
    stream << map.seed << ' ' << map.tiles.size() << '\n';
    for (const auto& [pos, tile] : map.tiles)
    {
        stream << "  " << pos << ' ' << tile << '\n';
    }
    return stream;
}

istream& operator>>(istream& stream, Map& map)
{
    size_t numTiles;
    stream >> map.seed >> numTiles;
    map.tiles.reserve(numTiles);
    for (size_t i = 0; i < numTiles; ++i)
    {
        IVec2 pos;
        Tile tile;
        stream >> pos >> tile;
        map.tiles.emplace(pos, tile);
    }
    return stream;
}
