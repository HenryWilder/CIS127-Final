#include "Utilities.h"

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
    cout << query << "\n> ";
    string input;
    getline(cin, input);
    return input;
}

vector<string>::const_iterator Prompt(const string& query, const vector<string>& options)
{
    cout << query;
    for (const auto& option : options)
    {
        string optText = option;
        cout << "\n: " << Colored(optText, GOLD);
    }
    cout << '\n';

    auto it = options.end();
    while (it == options.end())
    {
        cout << "> ";
        string input;
        getline(cin, input);
        it = find(options.begin(), options.end(), input);
        if (it == options.end() && input == "quit")
        {
            return options.end();
        }
    }

    return it;
}
