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
            
            vector<string> commandOptions;
            if (player.health.statuses.Has(StatusEffects::Tree))
            {
                commandOptions = { "quit", "restart" };
            }
            else
            {
                commandOptions = { "move", "use" };
            }
            
            string cmd = Prompt("What would you like to do?", commandOptions, { "quit", "restart" });
            
            // Top-level commands
            if (cmd == "move")
            {
                (void)Prompt("Where would you like to move?", { "left", "right", "forward" });
                surroundings.ReRoll(); // choice is an illusion :P
            }
            else if (cmd == "use")
            {
                vector<string> actionOptions = { "talk", "grab" };
                if (!player.inventory.IsEmpty())
                {
                    actionOptions.push_back("item");
                }
                
                // Request
                string action = Prompt("What would you like to use?", actionOptions);
                
                if (action == "item")
                {
                    do
                    {
                        action = player.inventory.Prompt("Which item?");
                        
                        if (action == "phonenumber")
                        {
                            cout << "If only you had a phone...\n";
                        }
                    }
                    while (action == "phonenumber");
                    
                    player.inventory.TryRemove(action, 1);
                }
                
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
            else if (cmd == "quit")
            {
                isQuitting = true;
                break;
            }
            else if (cmd == "restart")
            {
                break;
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
