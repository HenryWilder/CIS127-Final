#include "utilities.hpp"

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