#include "LevelMap.h"
#include <iostream>

bool operator==(Gridspace a, Gridspace b)
{
    return a.x == b.x && a.y == b.y;
}

GridSpaceFlags Room::at(_In_range_(0, ROOM_WIDTH) int x, _In_range_(0, ROOM_HEIGHT) int y) const
{
    return grid[y][x];
}

GridSpaceFlags Room::at(Gridspace position) const
{
    return at(position.x, position.y);
}

GridSpaceFlags& Room::at(_In_range_(0, ROOM_WIDTH) int x, _In_range_(0, ROOM_HEIGHT) int y)
{
    return grid[y][x];
}

GridSpaceFlags& Room::at(Gridspace position)
{
    return at(position.x, position.y);
}

void Room::PrintRoom() const
{
    using std::cout;
    using std::endl;
    for (int y = 0; y < ROOM_HEIGHT; ++y)
    {
        for (int x = 0; x < ROOM_WIDTH; ++x)
        {
            GridSpaceFlags spaceFlags = at(x, y);
            switch (spaceFlags)
            {
            case GridSpaceFlags::GRIDSPACE_EMPTY: cout << ' '; break;
            case GridSpaceFlags::GRIDSPACE_WALL:  cout << '#'; break;
            case GridSpaceFlags::GRIDSPACE_DOOR:  cout << '@'; break;
            default: cout << '?'; break;
            }
        }
        cout << '\n';
    }
    cout << endl;
}

// Todo: make this more advanced
void Room::Generate()
{
    for (int y = 0; y < ROOM_HEIGHT; ++y)
    {
        for (int x = 0; x < ROOM_WIDTH; ++x)
        {
            bool isWall = rand() & 1;
            at(x, y) = isWall ? GridSpaceFlags::GRIDSPACE_WALL : GridSpaceFlags::GRIDSPACE_EMPTY;
        }
    }
}

void LevelMap::PrintMap() const
{
    // todo
}

void LevelMap::Generate(int numRooms)
{
    rooms.resize(numRooms);
    for (Room& room : rooms)
    {
        room.Generate();
    }
}
