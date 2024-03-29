#include "LevelMap.h"
#include "Visuals.h"
#include <iostream>
#include <ConsoleGraphics.h>

bool operator==(ivec2 a, ivec2 b)
{
    return a.x == b.x && a.y == b.y;
}

_Use_decl_annotations_ GridSpaceFlags Room::at(int x, int y) const
{
    return grid[y][x];
}

GridSpaceFlags Room::at(ivec2 position) const
{
    return at(position.x, position.y);
}

Room::Room(int x, int y) :
    mapX(x), mapY(y), grid{} {}

_Use_decl_annotations_ GridSpaceFlags& Room::at(int x, int y)
{
    return grid[y][x];
}

GridSpaceFlags& Room::at(ivec2 position)
{
    return at(position.x, position.y);
}

int Room::GetX() const
{
    return mapX;
}

int Room::GetY() const
{
    return mapY;
}

void Room::Print(float scale) const
{
    constexpr Color floorColor( 10,  10,  10);
    constexpr Color  wallColor(127, 127, 127);
    constexpr Color  doorColor( 63, 255, 127);
    for (size_t y = 0; y < ROOM_HEIGHT; ++y)
    {
        for (size_t x = 0; x < ROOM_WIDTH; ++x)
        {
            Color colorToUse;
            switch (grid[y][x])
            {
            default:
            case GridSpaceFlags::GRIDSPACE_EMPTY: colorToUse = floorColor; break;
            case GridSpaceFlags::GRIDSPACE_WALL:  colorToUse = wallColor;  break;
            case GridSpaceFlags::GRIDSPACE_DOOR:  colorToUse = doorColor;  break;
            }
            cg::DrawPixel(vec2(x, y), colorToUse);
        }
    }
}

// Todo: make this more advanced
void Room::Generate()
{
    // Outermost walls
    for (int y = 0; y < ROOM_HEIGHT; ++y)
    {
        at(             0, y) = GridSpaceFlags::GRIDSPACE_WALL;
        at(ROOM_WIDTH - 1, y) = GridSpaceFlags::GRIDSPACE_WALL;
    }
    for (int x = 1; x < ROOM_WIDTH - 1; ++x)
    {
        at(x,               0) = GridSpaceFlags::GRIDSPACE_WALL;
        at(x, ROOM_HEIGHT - 1) = GridSpaceFlags::GRIDSPACE_WALL;
    }

    // Inside walls
    for (int y = 1; y < ROOM_HEIGHT - 1; ++y)
    {
        for (int x = 1; x < ROOM_WIDTH - 1; ++x)
        {
            bool isWall = rand() & 1;
            at(x, y) = isWall ? GridSpaceFlags::GRIDSPACE_WALL : GridSpaceFlags::GRIDSPACE_EMPTY;
        }
    }
}

void LevelMap::Print(float scale) const
{
    for (size_t i = 0; i < rooms.size(); ++i)
    {
        rooms[i].Print();
    }
}

void LevelMap::Generate(size_t numRooms)
{
    rooms.reserve(numRooms);
    size_t xRoomsPerSide = std::max((size_t)ceil(sqrt(numRooms)), 1ull);
    size_t yRoomsPerSide = std::max((size_t)floor(sqrt(numRooms)), 1ull);
    for (size_t i = 0; i < numRooms; ++i)
    {
        int x = (int)(i % xRoomsPerSide);
        int y = (int)(i / xRoomsPerSide);
        rooms.push_back(Room(x, y));
        rooms.back().Generate();
    }
}

size_t LevelMap::NumRooms() const
{
    return rooms.size();
}

Room& LevelMap::GetRoom(size_t room)
{
    return rooms.at(room);
}
