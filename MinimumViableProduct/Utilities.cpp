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
        if (input == "") // presses enter without typing anything
        {
            continue;
        }
        else if (input == "save")
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

PromptOptionIter Prompt(const string& query, const PromptOptionList& options)
{
    size_t maxPromptOptionWidth = 0;
    for (const PromptOption& option : options)
    {
        maxPromptOptionWidth = std::max(maxPromptOptionWidth, option.input.size());
    }

    cout << query << '\n';
    for (const PromptOption& option : options)
    {
        const auto& [input, description] = option;
        cout << "] " << Colored<GOLD>(input) << string(maxPromptOptionWidth - input.size(), ' ');
        if (description.has_value())
        {
            cout << " - " << description.value();
        }
        cout << '\n';
    }

    PromptOptionIter it = options.end();
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
        it = find_if(options.begin(), options.end(), [input](PromptOption option) { return input == option.input; });
    }

    return it;
}
