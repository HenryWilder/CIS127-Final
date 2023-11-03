#include "utilities.hpp"

string Prompt(const string& prompt, const vector<string>& options)
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
        auto it = find(options.begin(), options.end(), input);
        if (it != options.end())
        {
            return *it;
        }
    }
}

string ChooseRandom(const vector<string>& options)
{
    return options.at(rand() % options.size());
}

bool isvowel(char ch)
{
    if (isalpha(ch))
    {
        if (islower(ch))
        {
            return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
        }
        else
        {
            return ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U';
        }
    }
    return false;
}

const char* NotImplementedException::what() const noexcept
{
    return "feature not implemented";
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
