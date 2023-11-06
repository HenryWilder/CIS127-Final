#include "utilities.hpp"

string Prompt(const string& prompt, const vector<string>& options, const vector<string>& hiddenOptions)
{
    cout << prompt;
    for (const string& opt : options)
    {
        cout << "\n- " << opt;
    }
    cout << endl;
    while (true) // repeats until return
    {
        string input;
        cout << "> ";
        getline(cin, input);
        if (any_of(options.begin(), options.end(), input) || any_of(hiddenOptions.begin(), hiddenOptions.end(), input))
        {
            return input;
        }
    }
}

string Prompt(const string& prompt)
{
    cout << prompt << endl;
    string input = "";
    while (input == "")
    {
        cout << "> ";
        getline(cin, input);
    }
    return input;
}

bool isvowel(char ch)
{
    constexpr const char vowels[] = { 'a', 'e', 'i', 'o', 'u' };
    return isalpha(ch) && any_of(begin(vowels), end(vowels), tolower(ch));
}

NotImplementedException::NotImplementedException(const string& caseIdentifier)
{
    cout << "[[Missing implementation for case \"" << caseIdentifier << "\"]]";
}

const char* NotImplementedException::what() const noexcept
{
    return "feature not implemented";
}

string ChooseRandom(const vector<string>& options)
{
    return options.at(rand() % options.size());
}

string ChooseRandom(vector<string>::const_iterator optionsBegin, vector<string>::const_iterator optionsEnd)
{
    return *(optionsBegin + (rand() % (optionsEnd - optionsBegin)));
}

bool DiceCheck(int chance, int outOf)
{
    return (rand() % outOf) < chance;
};

void EchoAction(const string& action, const string& target, const string& topicOrEffect)
{
    cout << "You ";
    
    if      (action == "talk"  ) cout << "had an interesting discussion regarding " << topicOrEffect << " with";
    else if (action == "grab"  ) cout << "grabbed";
    else if (action == "bread" ) cout << "gave a piece of bread to";
    else if (action == "sword" ) cout << "swung your sword at";
    else if (action == "potion") cout << "used a potion of " << topicOrEffect << " on";
    else if (action == "gold"  ) cout << "gave some gold to";
    else throw new NotImplementedException();
    
    cout << " ";
    
    if (target == "self") cout << "yourself";
    else cout << "the " << target;
    
    cout << ".\n";
}
