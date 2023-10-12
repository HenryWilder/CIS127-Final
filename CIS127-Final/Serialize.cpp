#include "Serialize.h"
#include <fstream>
#include <iostream>
using std::istream;
using std::ifstream;
using std::ostream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::getline;

// I have elected not to have backwards compatibility between save versions.
// This should save on complexity.

struct VersionNumber
{
    unsigned major, minor, patch;
};

constexpr bool operator!=(const VersionNumber& a, const VersionNumber& b)
{
    return a.major != b.major || a.minor != b.minor || a.patch != b.patch;
}

constexpr VersionNumber PROGRAM_SAVE_VERSION = { 0, 1, 0 };

istream& operator>>(istream& stream, VersionNumber& version)
{
    return ((stream >> version.major).ignore(1) >> version.minor).ignore(1) >> version.patch;
}

ostream& operator<<(ostream& stream, const VersionNumber& version)
{
    return stream << version.major << '.' << version.minor << '.' << version.patch;
}

bool Save(const string& filename, const SaveData& data)
{
    ofstream ofs;
    ofs.open(filename, ofstream::out | ofstream::trunc);

    ofs << PROGRAM_SAVE_VERSION << '\n'; // Version number
    ofs << data.playerName << '\n';
    // Todo

    ofs.close();
    return true;
}

bool Load(const string& filename, SaveData& data)
{
    ifstream ifs;
    ifs.open(filename, ifstream::in);

    if (ifs)
    {
        VersionNumber fileVersion;
        ifs >> fileVersion;
        if (fileVersion != PROGRAM_SAVE_VERSION)
        {
            ifs.close();
            return false;
        }

        getline(ifs, data.playerName);
        // Todo
        
        ifs.close();
        return true;
    }
    else
    {
        cout << "Error: File not found." << endl;
        return false;
    }
}

ConfigData config;

void SaveConfigData()
{
    ofstream ofs;
    ofs.open(CONFIG_FILENAME, ofstream::out | ofstream::trunc);

    ofs << config.currentSaveIndex;
    for (string saveName : config.saveNames)
    {
        ofs << saveName << '\n';
    }

    ofs.close();
}

void LoadConfigData()
{
    ifstream ifs;
    ifs.open(CONFIG_FILENAME, ifstream::in);

    if (ifs)
    {
        ifs >> config.currentSaveIndex;
        ifs.ignore(256, '\n'); // Consume rest of line
        string saveName;
        while (getline(ifs, saveName))
        {
            config.saveNames.push_back(saveName);
        }

        ifs.close();
    }
}

bool ConfigData::SaveExists() const
{
    return !saveNames.empty();
}

const string& ConfigData::CurrentSaveName() const
{
    _ASSERT_EXPR(saveNames.empty(), "Cannot get current save name if there are no save names.");
    _ASSERT_EXPR(currentSaveIndex != -1, "currentSaveIndex is uninitialized.");
    _ASSERT_EXPR(currentSaveIndex >= (int)saveNames.size(), "currentSaveIndex is beyond the range of valid save files.");
    return saveNames[currentSaveIndex];
}
