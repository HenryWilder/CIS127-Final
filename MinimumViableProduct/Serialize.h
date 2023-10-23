#pragma once
#include "Utilities.h"
#include "Inventory.h"
#include "Character.h"
#include "Map.h"

struct SaveData
{
    SaveData() = default;
    SaveData(Player player); // New game constructor

    void Save() const;
    bool Load();

    Player player;
    Map map;
};

extern SaveData data;
