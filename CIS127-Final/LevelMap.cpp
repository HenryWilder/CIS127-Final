#include "LevelMap.h"
#include "Visuals.h"
#include <iostream>

bool operator==(Gridspace a, Gridspace b)
{
    return a.x == b.x && a.y == b.y;
}

_Use_decl_annotations_ GridSpaceFlags Room::at(int x, int y) const
{
    return grid[y][x];
}

GridSpaceFlags Room::at(Gridspace position) const
{
    return at(position.x, position.y);
}

_Use_decl_annotations_ GridSpaceFlags& Room::at(int x, int y)
{
    return grid[y][x];
}

GridSpaceFlags& Room::at(Gridspace position)
{
    return at(position.x, position.y);
}

void Room::PrintRoom(float scale) const
{
    TextureGrayscale tex(ROOM_WIDTH * scale, ROOM_HEIGHT * scale);
    vec2 resolution(ROOM_WIDTH, ROOM_HEIGHT);
    const Room* _this = this;
    tex.ApplyFragmentShader([_this, resolution](vec2 fragTexCoord)
    {
        vec2 gridPos = fragTexCoord * resolution;
        GridSpaceFlags spaceFlags = _this->at((size_t)gridPos.x, (size_t)gridPos.y);
        switch (spaceFlags)
        {
        case GridSpaceFlags::GRIDSPACE_EMPTY: return 0.0f;
        case GridSpaceFlags::GRIDSPACE_WALL:  return 0.5f;
        case GridSpaceFlags::GRIDSPACE_DOOR:  return 1.0f;
        }
    });
    tex.PrintIso();
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

void LevelMap::PrintMap() const
{
    // todo
}

size_t LevelMap::NumRooms() const
{
    return rooms.size();
}

_Use_decl_annotations_ void LevelMap::Generate(size_t numRooms)
{
    rooms.resize(numRooms);
    for (Room& room : rooms)
    {
        room.Generate();
    }
}

Room& LevelMap::GetRoom(size_t room)
{
    return rooms.at(room);
}
