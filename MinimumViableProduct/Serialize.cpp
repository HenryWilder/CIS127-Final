#include "Serialize.h"

SaveData::SaveData(Player player) :
    player(player) {}

const char* filename = "save.txt";

void SaveData::Save() const
{
    cout << Colored("[Saving...]", SKYBLUE);
    ofstream ofs(filename, ofstream::out | ofstream::trunc);
    if (ofs.is_open())
    {
        ofs << player;
        ofs << map;

        ofs.close();
        cout << Colored("[Save successful]\n", SKYBLUE);
        return;
    }
    cout << Colored("[Save failed]\n", RED);
}

bool SaveData::Load()
{
    cout << Colored("[Loading...]", SKYBLUE);
    ifstream ifs(filename, ofstream::in);
    if (ifs.is_open())
    {
        ifs >> player;
        ifs >> map;

        ifs.close();
        cout << Colored("[Load successful]\n", SKYBLUE);
        return true;
    }
    cout << Colored("[Load failed]\n", RED);
    return false;
}

SaveData data;
