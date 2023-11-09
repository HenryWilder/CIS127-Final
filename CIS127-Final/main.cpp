#include "utilities.hpp"
#include "randomness.hpp"
#include "Prompt.hpp"
#include "Enums.hpp"
#include "Interactable.hpp"
#include "Baker.hpp"
#include "Blacksmith.hpp"
#include "Wizard.hpp"
#include "Monster.hpp"
#include "Door.hpp"
#include "Components.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "serialization.hpp"

const unordered_map<wchar_t, char> EXTENDED_ASCII_MAP =
{
    { L'░', '\xB0' }, { L'▒', '\xB1' }, { L'▓', '\xB2' }, { L'│', '\xB3' }, { L'┤', '\xB4' }, { L'╡', '\xB5' }, { L'╢', '\xB6' }, { L'╖', '\xB7' },
    { L'╕', '\xB8' }, { L'╣', '\xB9' }, { L'║', '\xBA' }, { L'╗', '\xBB' }, { L'╝', '\xBC' }, { L'╜', '\xBD' }, { L'╛', '\xBE' }, { L'┐', '\xBF' },

    { L'└', '\xC0' }, { L'┴', '\xC1' }, { L'┬', '\xC2' }, { L'├', '\xC3' }, { L'─', '\xC4' }, { L'┼', '\xC5' }, { L'╞', '\xC6' }, { L'╟', '\xC7' },
    { L'╚', '\xC8' }, { L'╔', '\xC9' }, { L'╩', '\xCA' }, { L'╦', '\xCB' }, { L'╠', '\xCC' }, { L'═', '\xCD' }, { L'╬', '\xCE' }, { L'╧', '\xCF' },

    { L'╨', '\xD0' }, { L'╤', '\xD1' }, { L'╥', '\xD2' }, { L'╙', '\xD3' }, { L'╘', '\xD4' }, { L'╒', '\xD5' }, { L'╓', '\xD6' }, { L'╫', '\xD7' },
    { L'╪', '\xD8' }, { L'┘', '\xD9' }, { L'┌', '\xDA' }, { L'█', '\xDB' }, { L'▄', '\xDC' }, { L'▌', '\xDD' }, { L'▐', '\xDE' }, { L'▀', '\xDF' },
};

char ExtendedAscii(wchar_t ch)
{
    auto it = EXTENDED_ASCII_MAP.find(ch);
    return it != EXTENDED_ASCII_MAP.end() ? it->second : (char)ch;
}

void PrintInstructions()
{
    constexpr const wchar_t* instructions = LR"(
 ┌─[ Methods of input ]───────────────────────────────┐
 │                                                    │
 │ ┌─[ Single recognized word ]──────────────┐        │
 │ │                                         │        │
 │ ▐ Prompts with a list of recognized words │        │
 │ │                                         │        │
 │ │ ┌─[ Examples ]─┐                        │        │
 │ │ │ > use        │                        │        │
 │ │ │ > bread      │                        │        │
 │ │ │ > myself     │                        │        │
 │ │ ╞══════════════╡                        │        │
 │ │ │ > grab       │                        │        │
 │ │ │ > door       │                        │        │
 │ │ ╞══════════════╡                        │        │
 │ │ │ > move       │                        │        │
 │ │ │ > left       │                        │        │
 │ │ └──────────────┘                        │        │
 │ └─────────────────────────────────────────┘        │
 ╞════════════════════════════════════════════════════╡
 │                                                    │
 │ ┌─[ Simple sentence with recognized words ]──────┐ │
 │ │                                                │ │
 │ ▐ Unrecognized words are ignored                 │ │
 │ │                                                │ │
 │ │ ┌─[ Examples ]───────────────────────────────┐ │ │
 │ │ │ > use some of the tasty bread on myself    │ │ │
 │ │ ╞════════════════════════════════════════════╡ │ │
 │ │ │ > quickly grab the sturdy door and open it │ │ │
 │ │ ╞════════════════════════════════════════════╡ │ │
 │ │ │ > move to the left                         │ │ │
 │ │ └────────────────────────────────────────────┘ │ │
 │ └────────────────────────────────────────────────┘ │
 ╞════════════════════════════════════════════════════╡
 │ You can type 'quit' or 'restart' at any time.      │
 └────────────────────────────────────────────────────┘
)";
        
    for (const wchar_t* chPtr = instructions; *chPtr; ++chPtr)
    {
        cout << ExtendedAscii(*chPtr);
    }
    cout << endl;
}

void HorizontalRule()
{
    constexpr char dash = '\xC4'; // ─
    cout << '\n';
    for (size_t i = 0; i < 100; ++i)
    {
        cout << dash;
    }
    cout << "\n";
}

int main()
{    
    bool isQuitting = false;
    bool isRestarting = false;
    
    while (!isQuitting)
    {
        // Load the game
        LoadWithPrompt();
        
        PrintInstructions();

        // Game loop
        while (!isQuitting && !isRestarting)
        {
            if (player.health.IsDead())
            {
                cout << "Your health has dropped to zero and you have died.\n";
                isQuitting = !(isRestarting = (bool)boolean.Prompt("Would you like to start again?"));
                break;
            }
            
            Action action;
            {
                const array<Action, 2> alwaysAvailable = { Action::Quit, Action::Restart };

                if (!player.health.statuses.Has(StatusEffects::Tree))
                {
                    vector<Action> options =
                    {
                        Action::Move,
                        Action::Talk,
                        Action::Grab,
                        Action::Items,
                        Action::Near,
                    };
                    if (!player.inventory.IsEmpty())
                    {
                        options.insert(options.begin() + 3, Action::Use);
                    }
                    action = PromptOptionWithHidden("What would you like to do?", options, alwaysAvailable);
                }
                else // Soft locked
                {
                    action = PromptOption("You are a tree.", alwaysAvailable);
                }
            }

            switch (action)
            {
            case Action::Move:
            {
                Direction direction = directions.Prompt("Where would you like to move?");
                surroundings.ReRoll(); // choice is an illusion :P

                HorizontalRule();
                cout << '\n';
                cout << format("You walked {} into another room.\n", directions.ValueAt(direction));

                if (!surroundings.IsEmpty())
                {
                    StreamList::Push("You see ", "a ", nullptr, "", ", ", " and ", ".", false);
                    surroundings.Print();
                    StreamList::Pop();
                }
                else
                {
                    cout << "You are alone with your thoughts.";
                }
                cout << '\n';
                HorizontalRule();
            }
                break;

            case Action::Talk:
            {
                EntityTypeInfo_t target = entityTypes.At(surroundings.Prompt("Who/what would you like to talk to?"));
                Entity& targetObject = surroundings.Get(target);

                TopicInfo_t topic = topics.Random();

                HorizontalRule();
                cout << '\n';
                cout << format("You had an interesting discussion with {} regarding {}.\n", target.full, topic.full);

                targetObject.DoInteraction_Talk(topic);
                cout << '\n';
                HorizontalRule();
            }
                break;

            case Action::Grab:
            {
                EntityTypeInfo_t target = entityTypes.At(surroundings.Prompt("Who/what would you like to grab?"));
                Entity& targetObject = surroundings.Get(target);

                HorizontalRule();
                cout << '\n';
                cout << format("You grabbed {}.\n", target.full);

                targetObject.DoInteraction_Grab();
                cout << '\n';
                HorizontalRule();
            }
                break;

            case Action::Use:
            {
                Item item;
                item = player.inventory.Prompt("Which item would you like to use?");

                if (item == Item::Phonenumber)
                {
                    cout << "If only you had a phone...\n";
                    break;
                }

                player.inventory.TryRemove(item, 1);

                EntityTypeInfo_t target = entityTypes.At(surroundings.Prompt(format("Who/what would you like to use your {} on?", items.ValueAt(item))));
                Entity& targetObject = surroundings.Get(target);

                PotionInfo_t potion = potions.Random(); // May go unused

                HorizontalRule();
                cout << '\n';
                switch (item)
                {
                case Item::Bread:  cout << format("You gave a piece of bread to {}.\n",            target.full); targetObject.DoInteraction_Bread ();       break;
                case Item::Sword:  cout << format("You swung your sword at {}.\n",                 target.full); targetObject.DoInteraction_Sword ();       break;
                case Item::Gold:   cout << format("You gave some gold to {}.\n",                   target.full); targetObject.DoInteraction_Gold  ();       break;
                case Item::Potion: cout << format("You used a potion of {} on {}.\n", potion.full, target.full); targetObject.DoInteraction_Potion(potion); break;
                default: throw new NotImplementedException(items.KeyAt(item));
                }
                cout << '\n';
                HorizontalRule();
            }
                break;

            case Action::Items:
                StreamList::Push("[\n", "  { ", ": ", " }", ",\n", nullptr, "\n]", true);

                HorizontalRule();
                cout << '\n';
                cout << "Your current inventory:\n";
                player.inventory.Print();
                cout << '\n';
                HorizontalRule();

                StreamList::Pop();
                break;

            case Action::Near:
                StreamList::Push("[\n", "  ", nullptr, "", ",\n", nullptr, "\n]", true);

                HorizontalRule();
                cout << '\n';
                cout << "Your current surroundings:\n";
                surroundings.Print();
                cout << '\n';
                HorizontalRule();

                StreamList::Pop();
                break;

            case Action::Quit:
                isQuitting = true;
                break;

            case Action::Restart:
                isRestarting = true;
                break;

            default:
                throw new NotImplementedException(actions.KeyAt(action));
            }

            if (isQuitting || isRestarting)
            {
                break;
            }
            
            cout << endl;
        }
    }
    
    // Save the game
    Save();
    
    return 0;
}
