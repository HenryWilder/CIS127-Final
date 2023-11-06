#include "utilities.hpp"
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
    
    
    // Any given NPC has a random chance of being in a particular collective.
    Collective::Create("western_expance",           "citizens of the Western Expanse");
    Collective::Create("twisting_vacancy",          "Valley of the Twisting Vacancy");
    Collective::Create("infinite_pathways",         "Children of the Pond of Infinite Pathways");
    Collective::Create("northern_seed",             "Followers of the Northern Seed");
    Collective::Create("eastern_swampfire",         "Earthen Swampfire district of the Northeastern Bishop's Domain");
    Collective::Create("critical_malstrom",         "Beholders of the Critical Malstrom");
    Collective::Create("elder_kings",               "Nursing Home of the Elder Kings");
    Collective::Create("dead_king_citadel",         "Holdout Clan of the Dead King's Fallen Citadel");
    Collective::Create("babypunching_puppykickers", "League of Babypunching Puppykickers.");
    Collective::Create("monsters",                  "monsters");
    
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
                    topicOrEffect = ChooseRandom(topics);
                }
                else if (action == "potion")
                {
                    switch (player.luck.Check())
                    {
                        case +1:
                            if (target == "self")
                            {
                                vector lowerHalf = vector(potionEffects.begin(), potionEffects.begin() + potionEffects.size() / 2);
                                topicOrEffect = ChooseRandom(lowerHalf);
                            }
                            else
                            {
                                vector upperHalf = vector(potionEffects.begin() + potionEffects.size() / 2, potionEffects.end());
                                topicOrEffect = ChooseRandom(upperHalf);
                            }
                            break;
                            
                        default:
                            topicOrEffect = ChooseRandom(potionEffects);
                            break;
                    }
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
