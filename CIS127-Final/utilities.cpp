#include "utilities.hpp"

string Prompt()
{
    loop
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

string Prompt(const string& prompt, const vector<string>& options, const vector<string>& hiddenOptions)
{
    cout << prompt << '\n';
    List(options);
    auto _Pred = [&](const string& input)
    {
        return any_of(options.begin(), options.end(), input) || any_of(hiddenOptions.begin(), hiddenOptions.end(), input);
    };
    return PromptUntil(_Pred);
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

bool DiceCheck(int chance, int outOf)
{
    return (rand() % outOf) < chance;
}

bool Chance(float chance01)
{
    if      (chance01 >= 0.999f) return true;
    else if (chance01 <= 0.001f) return false;
    return rand() <= (int)roundf(chance01 * (float)RAND_MAX);
}

bool CoinFlip()
{
    return rand() & 1;
}

bool AdvantagedCoinFlip()
{
    return rand() & 3;
}

bool DisadvantagedCoinFlip()
{
    return (rand() & 3) == 3;
}

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
