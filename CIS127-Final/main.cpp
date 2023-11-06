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

    const string ITEM_BREAD  = "bread";  // Heals
    const string ITEM_SWORD  = "sword";  // Damages; quantity represents durability
    const string ITEM_POTION = "potion"; // Effects are random
    const string ITEM_GOLD   = "gold";   // 100% chance of getting items from NPCs willing to trade (higher chance than bread/talking)
    
    const vector<string> itemNames = {
        ITEM_BREAD,
        ITEM_SWORD,
        ITEM_POTION,
        ITEM_GOLD,
    };
    
    const string POTION_EFFECT_PREDICT = "predict"; // Gives luck, providing an upper hand in anything the player is doing
    const string POTION_EFFECT_HEAL    = "heal";    // Heals the target
    const string POTION_EFFECT_WATER   = "water";   // Douces the target in water - useless because fire is not DOT and there are no fire elementals
    const string POTION_EFFECT_WISH    = "wish";    // Gives the target a wish; wish is random unless the target is the player
    const string POTION_EFFECT_DUCKS   = "ducks";   // Summons ducks - useless
    const string POTION_EFFECT_FORCE   = "force";   // Pushes the target away - rerolls surroundings if player, removes target from surroundings otherwise
    const string POTION_EFFECT_SALT    = "salt";    // Random chance to kill a monster
    const string POTION_EFFECT_ANTS    = "ants";    // Summons ants - useless
    const string POTION_EFFECT_DEMON   = "demon";   // Summons a random demon to attack the target
    const string POTION_EFFECT_FIRE    = "fire";    // Deals some damage
    const string POTION_EFFECT_EXPLODE = "explode"; // Deals high damage; can 1-hit most things including doors
    const string POTION_EFFECT_TREE    = "tree";    // Turns the target into a tree, soft-locking the game if the target was the player.
    
    // Problem: Softlocking the game is a major enough risk that none of the effects are worth using potions on yourself.
    // Perhaps there could be a chance that NPCs can un-tree the player after one turn? Throwing away a turn is less destructive than a full-on soft-lock.

    // Sorted from most to least positive effects.
    // First half are considered lucky when targeting a friend,
    // second half are considered lucky when targeting an enemy
    const vector<string> potionEffects = {
        POTION_EFFECT_PREDICT,
        POTION_EFFECT_HEAL,
        POTION_EFFECT_WATER,
        POTION_EFFECT_WISH,
        POTION_EFFECT_DUCKS,
        POTION_EFFECT_FORCE,
        POTION_EFFECT_SALT,
        POTION_EFFECT_ANTS,
        POTION_EFFECT_DEMON,
        POTION_EFFECT_FIRE,
        POTION_EFFECT_EXPLODE,
        POTION_EFFECT_TREE,
    };
    
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
