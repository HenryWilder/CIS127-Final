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

Room::Room(int x, int y) :
    mapX(x), mapY(y) {}

_Use_decl_annotations_ GridSpaceFlags& Room::at(int x, int y)
{
    return grid[y][x];
}

GridSpaceFlags& Room::at(Gridspace position)
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

void Room::GetRoomTexture(TextureGrayscale& tex) const
{
    vec2 resolution(ROOM_WIDTH - 1, ROOM_HEIGHT - 1);
    const Room* _this = this;
    tex.ApplyFragmentShader([_this, resolution](vec2 fragTexCoord)
    {
        vec2 gridPos = fragTexCoord * resolution;
        GridSpaceFlags spaceFlags = _this->at((size_t)(gridPos.x + 0.5f), (size_t)(gridPos.y + 0.5f));
        switch (spaceFlags)
        {
        case GridSpaceFlags::GRIDSPACE_EMPTY: return 0.0f;
        case GridSpaceFlags::GRIDSPACE_WALL:  return 1.0f;
        case GridSpaceFlags::GRIDSPACE_DOOR:  return 0.5f;
        }
    });
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

void LevelMap::GetMapTexture(TextureGrayscale& tex) const
{
    float roomWidth = 1.0f / ceilf(sqrt(rooms.size()));
    quad baseQuad = {
        vec2(0),
        vec2(roomWidth, 0),
        vec2(roomWidth),
        vec2(0, roomWidth)
    };
    for (const Room& room : rooms)
    {
        TextureGrayscale roomTex(ROOM_WIDTH, ROOM_HEIGHT);
        room.GetRoomTexture(roomTex);
        vec2 pos = vec2(room.GetX(), room.GetY()) * roomWidth;
        tex.Draw(roomTex, Offset(baseQuad, pos));
    }
}

void LevelMap::Generate(size_t numRooms)
{
    rooms.reserve(numRooms);
    size_t roomsPerSide = (size_t)ceil(sqrt(rooms.size()));
    if (roomsPerSide < 1) roomsPerSide = 1;
    for (size_t i = 0; i < numRooms; ++i)
    {
        rooms.push_back(Room(i % roomsPerSide, i / roomsPerSide));
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
