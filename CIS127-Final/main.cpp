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
    const string TOPIC_WINEFISH      = "the effects of water-wine alchemy on the local fish population";
    const string TOPIC_SKELESTOCK    = "the volatile stock price of enchanted skeleton armor";
    const string TOPIC_WP_SIEGE      = "how many woodpeckers it would take to breach the castle wall";
    const string TOPIC_NECROFARM     = "the ethicacy of using necromancy in farming-related fields";
    const string TOPIC_BLOODMOON     = "when the next blood moon may occur";
    const string TOPIC_MOLE_MNT      = "whether enough moles could in fact make a mountain out of their hill";
    const string TOPIC_MF_RECIPE     = "what recipes to use for preparing mind flayer";
    const string TOPIC_GOD_FISTFIGHT = "which gods could win in a fistfight against each other";
    const string TOPIC_THESEUS       = "why Theseus has been getting so fussy about their ship lately";
    const string TOPIC_BS_TELEKEN    = "what form of telekenesis would be the most effective for a blacksmith to use";
    const string TOPIC_NO_GARLIC     = "how the elder wizards should handle the recent garlic & holy water defecits";
    const string TOPIC_PENGUIN_BTL   = "whether the Old Realm needs more supplies or troops to survive their war of attrition against the Penguin Guild";
    const string TOPIC_PET_MNTL_HP   = "the effects of mind-altering spells on the mental health of familiars";
    const string TOPIC_WOODCHUCK     = "the quantity of wood throwable by a woodchuck in a hypothetical scenario that such a feat was possible for the creature";
    
    const array<string, 14> topics = {
        TOPIC_WINEFISH,
        TOPIC_SKELESTOCK,
        TOPIC_WP_SIEGE,
        TOPIC_NECROFARM,
        TOPIC_BLOODMOON,
        TOPIC_MOLE_MNT,
        TOPIC_MF_RECIPE,
        TOPIC_GOD_FISTFIGHT,
        TOPIC_THESEUS,
        TOPIC_BS_TELEKEN,
        TOPIC_NO_GARLIC,
        TOPIC_PENGUIN_BTL,
        TOPIC_PET_MNTL_HP,
        TOPIC_WOODCHUCK,
    };

    const string ITEM_BREAD  = "bread";  // Heals
    const string ITEM_SWORD  = "sword";  // Damages; quantity represents durability
    const string ITEM_POTION = "potion"; // Effects are random
    const string ITEM_GOLD   = "gold";   // 100% chance of getting items from NPCs willing to trade (higher chance than bread/talking)
    
    const array<string, 4> itemNames = {
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
    const array<string, 12> potionEffects = {
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
        Load();
        
        cout << endl;
        
        // Game loop
        while (true)
        {
            surroundings.Print();
            player.inventory.Print();
            cout << endl;
            
            vector<string> commandOptions;
            if (player.health.statuses.Has(StatusEffects::Tree))
            {
                commandOptions = { "quit", "restart" };
            }
            else
            {
                commandOptions = { "move", "use", "quit", "restart" };
            }
            
            string cmd = Prompt("What would you like to do?", commandOptions);
            
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
                    action = player.inventory.Prompt("Which item?");
                    player.inventory.TryRemove(action, 1);
                }
                
                if (action == "phonenumber")
                {
                    cout << "If only you had a phone...\n\n";
                    continue;
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
                    constexpr int potionEffectsHalfSize = potionEffects.size() / 2;
                    switch (player.luck.Check())
                    {
                        case +1:
                            if (target == "self") // Lower half
                            {
                                topicOrEffect = potionEffects.at(rand() % potionEffectsHalfSize);
                            }
                            else // Upper half
                            {
                                topicOrEffect = potionEffects.at(potionEffectsHalfSize + (rand() % (potionEffects.size() - potionEffectsHalfSize)));
                            }
                            break;
                            
                        default:
                            topicOrEffect = ChooseRandom(potionEffects);
                            break;
                    }
                }
                
                EchoAction(action, target, topicOrEffect);
                
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
