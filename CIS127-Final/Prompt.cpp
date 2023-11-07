#include "Prompt.hpp"

namespace stream_list
{
    const char* _Stream_List_Item_Prefix   ::value = "- ";
    const char* _Stream_List_Item_Separator::value = "\n";
    const char* _Stream_List_Pair_Separator::value = ": ";
}

string Prompt()
{
    while (true)
    {
        cout << "> ";
        string input;
        getline(cin, input);
        if (!IsEmptyOrWhitespace(input))
        {
            return Trim(input);
        }
    }
}

string Prompt(const string& prompt)
{
    cout << prompt << '\n';
    return Prompt();
}
