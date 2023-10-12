#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

// Minimum of 6 items
struct SaveData
{
    string playerName = "Ugene Plunderbuss";
    // todo: player inventory
    // todo: player room
};

struct ConfigData
{
    int currentSaveIndex = -1;
    vector<string> saveNames = {};

    bool SaveExists() const;
    const string& CurrentSaveName() const;
};

constexpr const char* CONFIG_FILENAME = "config.txt";
extern ConfigData config;

// Saves data from from config to [CONFIG_FILENAME].
void SaveConfigData();

// Loads data from [CONFIG_FILENAME] to config.
void LoadConfigData();

bool Save(const string& filename, const SaveData& data);
bool Load(const string& filename, SaveData& data);
