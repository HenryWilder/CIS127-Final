#pragma once
#include "Utilities.h"
#include "Inventory.h"
#include "Character.h"

struct SaveData
{
    SaveData();
    SaveData(Player player); // New game constructor

    void Save(const char* filename) const;
    bool Load(const char* filename);

    Player player;
};

extern SaveData data;
