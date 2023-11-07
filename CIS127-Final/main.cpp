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
    const char* TURN_SPLIT = "---------------------------------";
        
    const string COLLECTIVE_BABYPUNCHING_PUPPYKICKERS_NOTE = "You feel a bit of pity for the group's unfortunate name, a poor translation from the fennecborns' native tongue for \"soft-handed littlepaw-walkers\". Maybe with your newfound sway, you can convince them to change their name";
    
    bool isQuitting = false;
    
    while (!isQuitting)
    {
        // Load the game
        LoadWithPrompt();
        
        cout << endl;
        
        // Game loop
        while (true)
        {
            cout << TURN_SPLIT << "\n\n";
            surroundings.Print();
            cout << endl;
            player.inventory.Print();
            cout << endl;
            
            vector<Action> commandOptions;
            if (player.health.statuses.Has(StatusEffects::Tree))
            {
                commandOptions = { Action::Quit, Action::Restart };
            }
            else
            {
                commandOptions = { Action::Move, Action::Talk, Action::Grab };
                if (!player.inventory.IsEmpty())
                {
                    commandOptions.push_back(Action::Use);
                }
            }
            
            Action action = Prompt<Action>("What would you like to do?", commandOptions, initializer_list{ Action::Quit, Action::Restart });

            switch (action)
            {
            case Action::Move:
                (void)directions.Prompt("Where would you like to move?");
                surroundings.ReRoll(); // choice is an illusion :P
                break;

            case Action::Talk:
            {
                auto target = surroundings.Prompt("To who?");
            }
                break;

            case Action::Grab:
                break;

            case Action::Use:
            {
                Item item;
                item = player.inventory.Prompt("Which item?");

                if (action == "phonenumber")
                {
                    cout << "If only you had a phone...\n";
                    break;
                }

                player.inventory.TryRemove(item, 1);
            }
                break;

            case Action::Quit:
                isQuitting = true;
                // Todo: break out of loop
                break;

            case Action::Restart:
                // Todo: break out of loop
                break;

            default: throw new NotImplementedException(actions.KeyAt(action));
            }
            
            else if ()
            {                
                auto target = surroundings.Prompt("On what?");
                
                string topicOrEffect;

                // Pick topic/effect
                if (action == "talk")
                {
                    topicOrEffect = topics.RandomKey();
                }
                else if (action == "potion")
                {
                    topicOrEffect = potions.RandomKey();
                }
                
                cout << "\n" << TURN_SPLIT << "\n\n";
                EchoAction(action, target, topicOrEffect);
                cout << endl;
                
                if (target == "self")
                {
                    player.DoInteraction(action, topicOrEffect);
                }
                else
                {
                    surroundings.Get(target).DoInteraction(action, topicOrEffect);
                }
                
                cout << endl;
            }
            
            cout << endl;
            
            if (player.health.IsDead() &&
                Prompt("Your health has dropped to zero and you have died.\nWould you like to start again?", { "yes", "no" }) == "yes")
            {
                break;
            }
        }
    }
    
    // Save the game
    Save();
    
    return 0;
}
