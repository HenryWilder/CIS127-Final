#include "Utilities.h"
#include "Serialize.h" // Allows us to use the SaveData `data`

ColoredText Colored(const string& text, _In_range_(0, 255) byte r, _In_range_(0, 255) byte g, _In_range_(0, 255) byte b)
{
    return { text, r, g, b };
}

ColoredText Colored(const string& text, _In_range_(0, 0xFFFFFF) int hex)
{
    return { text, (byte)(hex >> 020), (byte)(hex >> 010), (byte)(hex >> 000) };
}

ostream& operator<<(ostream& stream, const ColoredText& text)
{
    if constexpr (USE_COLORED_TEXT)
    {
        return stream << "\x1B[38;2;"
            << (unsigned int)text.r << ';'
            << (unsigned int)text.g << ';'
            << (unsigned int)text.b << 'm'
            << text.text << "\x1B[0m";
    }
    else
    {
        return stream << text.text;
    }
}


string PromptString(const string& query)
{
    cout << query << '\n';
    while (true)
    {
        cout << "> ";
        string input;
        getline(cin, input);
        if (input == "save")
        {
            data.Save();
        }
        else if (input == "quit")
        {
            data.Save();
            exit(0);
        }
        else
        {
            return input;
        }
    }
}

vector<string>::const_iterator Prompt(const string& query, const vector<string>& options)
{
    cout << query;
    for (const auto& option : options)
    {
        string optText = option;
        cout << "\n: " << Colored<GOLD>(optText);
    }
    cout << '\n';

    auto it = options.end();
    while (it == options.end())
    {
        cout << "> ";
        string input;
        getline(cin, input);
        if (it == options.end())
        {
            if (input == "save")
            {
                data.Save();
                continue;
            }
            if (input == "quit")
            {
                data.Save();
                exit(0);
            }
        }
        it = find(options.begin(), options.end(), input);
    }

    return it;
}
