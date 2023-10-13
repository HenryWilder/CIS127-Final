#pragma once
#include <vector>
#include <sal.h>
using std::vector;

constexpr size_t ROOM_WIDTH  = 16;
constexpr size_t ROOM_HEIGHT = 16;

struct Gridspace
{
    _Field_range_(0, ROOM_WIDTH)  int x;
    _Field_range_(0, ROOM_HEIGHT) int y;
};

bool operator==(Gridspace a, Gridspace b);

enum class Cardinal
{
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

enum class GridSpaceFlags
{
    GRIDSPACE_EMPTY,
    GRIDSPACE_WALL,
    GRIDSPACE_DOOR,
};

class Room
{
public:
    GridSpaceFlags at(_In_range_(0, ROOM_WIDTH - 1) int x, _In_range_(0, ROOM_HEIGHT - 1) int y) const;
    GridSpaceFlags at(Gridspace position) const;

    void PrintRoom() const;
    void Generate(); // Generates walls; doors are generated by LevelMap

private:
    GridSpaceFlags& at(_In_range_(0, ROOM_WIDTH - 1) int x, _In_range_(0, ROOM_HEIGHT - 1) int y);
    GridSpaceFlags& at(Gridspace position);
    GridSpaceFlags grid[ROOM_WIDTH][ROOM_HEIGHT];
};

class LevelMap
{
public:
    size_t NumRooms() const;
    Room& GetRoom(size_t room);

    void PrintMap() const;
    void Generate(_In_range_(0, rooms.size() - 1) size_t numRooms);

private:
    vector<Room> rooms;
};
