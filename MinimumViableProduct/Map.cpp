#include "Map.h"

void Map::DoMovement(Player* player)
{
    vector<string> options;

    // Will always have same number of elements as options
    // Tiles without entities will have nullptr
    vector<const Entity*> entityObjects;

    // Anonymous structure - easier to use thanks to C++17 structured binding
    struct { const char* name; int x, y; }
    constexpr directions[] =
    {
        { "left",  -1,  0 },
        { "right", +1,  0 },
        { "up",     0, -1 },
        { "down",   0, +1 },
    };

    // Add/remove movement options conditionally
    {
        for (size_t i = 0; i < _countof(directions); ++i)
        {
            const auto& [directionName, xOffset, yOffset] = directions[i];

            int xPrime = player->x + xOffset;
            int yPrime = player->y + yOffset;

            if (!IsValidTile(xPrime, yPrime))
            {
                continue;
            }

            const Entity* entity = GetEntityAtPosition(xPrime, yPrime);
            options.push_back(directionName);
            entityObjects.push_back(entity);

            if (entity) // Implied "!= nullptr" because nullptr is 0
            {
                options.back() += ' ' + entity->GetCategoryName();
            }
        }
    }

    // Handle input
    {
        auto it = Prompt("Where", options);
        string selectedName = *it;
        size_t selectedIndex = it - options.begin();

        // Explicit movement
        for (const auto& [directionName, xOffset, yOffset] : directions)
        {
            if (selectedName == directionName)
            {
                player->Move(xOffset, yOffset);
                return;
            }
        }

        // Entity interaction
        {
            const Entity* entity = entityObjects[selectedIndex];
            // todo
        }
    }
}

bool Map::IsValidTile(int x, int y) const
{
    // todo
    return true;
}

const Entity* Map::GetEntityAtPosition(int x, int y) const
{
    // todo
    return nullptr;
}

ostream& operator<<(ostream& stream, const Entity& map)
{
    // todo
    return stream;
}

istream& operator>>(istream& stream, Entity& map)
{
    // todo
    return stream;
}

ostream& operator<<(ostream& stream, const Map& map)
{
    // todo
    return stream;
}

istream& operator>>(istream& stream, Map& map)
{
    // todo
    return stream;
}
