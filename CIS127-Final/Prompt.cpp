#include "Prompt.hpp"


// Awaits a string from the user, prefixing each attempt with "> ".
// Able to take multiple inputs "at once" if the user inserts spaces between prompts.
string Prompt()
{
    while (true)
    {
        cout << "> ";
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
        cout << "> ";
        string input;
        cin >> input;
        while (cin.peek() == ' ')
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
