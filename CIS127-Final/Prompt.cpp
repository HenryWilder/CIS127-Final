#include "Prompt.hpp"

bool IsInputExisting()
{
    return cin.tellg() > 0 && cin.good() && cin.peek() == ' ';
}

// Awaits a string from the user, prefixing each attempt with "> ".
// Able to take multiple inputs "at once" if the user inserts spaces between prompts.
string Prompt()
{
    while (true)
    {
        if (!IsInputExisting())
        {
            cout << "> ";
        }
        string input;
        cin >> input;
        if (!IsEmptyOrWhitespace(input))
        {
            return Trim(input);
        }
    }
}

string PromptLine()
{
    while (true)
    {
        if (!IsInputExisting())
        {
            cout << "> ";
        }
        string input;
        cin >> input;
        while (IsInputExisting())
        {
            string word;
            cin >> word;
            input += ' ' + word;
        }
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

string PromptLine(const string& prompt)
{
    cout << prompt << '\n';
    return PromptLine();
}
