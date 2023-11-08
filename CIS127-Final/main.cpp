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

int main()
{
    constexpr const char TURN_SPLIT[] = "\n---------------------------------\n\n";
    
    bool isQuitting = false;
    bool isRestarting = false;
    
    while (!isQuitting)
    {
        // Load the game
        LoadWithPrompt();
        
        cout << R"(
Methods of input include:
 # Single recognized word.
 | ---[Examples]----------
 | > use
 | > bread
 | > myself
 | ---
 | > grab
 | > door
 | ---
 | > move
 | > left
 | -----------------------
 | 
 # Simple sentence containing recognized words.
 | Unrecognized words are ignored.
 | ---[Examples]-------------------------------
 | > use some of the tasty bread on myself
 | ---
 | > quickly grab the sturdy door and open it
 | ---
 | > move to the left
 | --------------------------------------------
)";

        // Game loop
        while (!isQuitting && !isRestarting)
        {
            cout << TURN_SPLIT;

            if (player.health.IsDead())
            {
                cout << "Your health has dropped to zero and you have died.\n";
                isQuitting = !(isRestarting = (bool)boolean.Prompt("Would you like to start again?"));
                break;
            }

            StreamList::Push(" [\n", "\n]\n\n", "  { ", " }", ",\n", Argument::Default, true);
            surroundings    .Print();
            player.inventory.Print();
            StreamList::Pop();
            
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
                        Action::Use
                    };
                    if (player.inventory.IsEmpty())
                    {
                        options.pop_back(); // Remove "Use"
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
            }
                break;

            case Action::Talk:
            {
                EntityTypeInfo_t target = entityTypes.At(surroundings.Prompt("Who/what would you like to talk to?"));
                Entity& targetObject = surroundings.Get(target);

                TopicInfo_t topic = topics.Random();

                cout << TURN_SPLIT;
                cout << format("You had an interesting discussion regarding {} with {}.\n", topic.full, target.full);

                targetObject.DoInteraction_Talk(topic);
            }
                break;

            case Action::Grab:
            {
                EntityTypeInfo_t target = entityTypes.At(surroundings.Prompt("Who/what would you like to grab?"));
                Entity& targetObject = surroundings.Get(target);

                cout << TURN_SPLIT;
                cout << format("You grabbed {}.\n", target.full);

                targetObject.DoInteraction_Grab();
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


                cout << "\n" << TURN_SPLIT;
                switch (item)
                {
                case Item::Bread:  cout << format("You gave a piece of bread to {}.\n",            target.full); targetObject.DoInteraction_Bread ();       break;
                case Item::Sword:  cout << format("You swung your sword at {}.\n",                 target.full); targetObject.DoInteraction_Sword ();       break;
                case Item::Gold:   cout << format("You gave some gold to {}.\n",                   target.full); targetObject.DoInteraction_Gold  ();       break;
                case Item::Potion: cout << format("You used a potion of {} on {}.\n", potion.full, target.full); targetObject.DoInteraction_Potion(potion); break;
                default: throw new NotImplementedException(items.KeyAt(item));
                }
            }
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
