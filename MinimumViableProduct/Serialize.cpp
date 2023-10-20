#include "Serialize.h"

SaveData::SaveData() = default;

SaveData::SaveData(Player player) :
    player(player) {}

void SaveData::Save(const char* filename) const
{
    cout << Colored("[Saving...]", SKYBLUE);
    ofstream ofs(filename, ofstream::trunc);
    if (ofs.is_open())
    {
        ofs << player;

        ofs.close();
        cout << Colored("[Save successful]\n", SKYBLUE);
        return;
    }
    cout << Colored("[Save failed]\n", RED);
}

bool SaveData::Load(const char* filename)
{
    cout << Colored("[Loading...]", SKYBLUE);
    ifstream ifs(filename);
    if (ifs.is_open())
    {
        ifs >> player;

        ifs.close();
        cout << Colored("[Load successful]\n", SKYBLUE);
        return true;
    }
    cout << Colored("[Load failed]\n", RED);
    return false;
}

SaveData data;
