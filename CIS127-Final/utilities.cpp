#include "utilities.hpp"

bool isvowel(char ch)
{
    constexpr const char vowels[] = { 'a', 'e', 'i', 'o', 'u' };
    return isalpha(ch) && any_of(begin(vowels), end(vowels), tolower(ch));
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
