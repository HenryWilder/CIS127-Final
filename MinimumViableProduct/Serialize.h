#pragma once
#include "Utilities.h"
#include "Inventory.h"
#include "Character.h"

struct SaveData
{
    SaveData() = default;
    SaveData(Player player); // New game constructor

    void Save() const;
    bool Load();

    Player player;
};

extern SaveData data;
