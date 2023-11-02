#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <map>
using namespace std;

string Prompt(const string& prompt, const vector<string>& options);
string PromptItem(const string& prompt, const map<string, int>& options);

void _RerollSurroundings(vector<string>& surroundings);

class NotImplementedException :
    public exception
{
    const char* what() const noexcept override
    {
        return "feature not implemented";
    }
};

string ChooseRandom(const vector<string>& options)
{
    return options.at(rand() % options.size());
}

template<size_t _Size>
string ChooseRandom(const array<string, _Size>& options)
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

int main()
{
#define TOPIC_WINEFISH      "the effects of water-wine alchemy on the local fish population"
#define TOPIC_SKELESTOCK    "the volatile stock price of enchanted skeleton armor"
#define TOPIC_WP_SIEGE      "how many woodpeckers it would take to breach the castle wall"
#define TOPIC_NECROFARM     "the ethicacy of using necromancy in farming-related fields"
#define TOPIC_BLOODMOON     "when the next blood moon may occur"
#define TOPIC_MOLE_MNT      "whether enough moles could in fact make a mountain out of their hill"
#define TOPIC_MF_RECIPE     "what recipes to use for preparing mind flayer"
#define TOPIC_GOD_FISTFIGHT "which gods could win in a fistfight against each other"
#define TOPIC_THESEUS       "why Theseus has been getting so fussy about their ship lately"
#define TOPIC_BS_TELEKEN    "what form of telekenesis would be the most effective for a blacksmith to use"
#define TOPIC_NO_GARLIC     "how the elder wizards should handle the recent garlic & holy water defecits"
#define TOPIC_PENGUIN_BTL   "whether the Old Realm needs more supplies or troops to survive their war of attrition against the Penguin Guild"
#define TOPIC_PET_MNTL_HP   "the effects of mind-altering spells on the mental health of familiars"
#define TOPIC_WOODCHUCK     "the quantity of wood throwable by a woodchuck in a hypothetical scenario that such a feat was possible for the creature"
    
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

    const array<string, 4> itemNames = {
        "bread",  // Heals
        "sword",  // Hurts stuff - Player can only have one at a time, number is durability (number of swings) remaining. Smiths can repair it.
        "potion", // Effects are random
        "gold",   // 100% chance of getting items from NPCs
    };

    // Sorted from most to least positive effects
    const array<string, 12> potionEffects = {
        "predict",
        "heal",
        "water",
        "wish",
        "ducks",
        "force",
        "salt",
        "ants",
        "demon",
        "fire",
        "explode",
        "tree",
    };
    
    const array<string, 10> collectives = {
        "citizens of the Western Expanse",
        "Valley of the Twisting Vacancy",
        "Children of the Pond of Infinite Pathways",
        "Followers of the Northern Seed",
        "Earthen Swampfire district of the Northeastern Bishop's Domain",
        "Beholders of the Critical Malstrum",
        "Nursing Home of the Elder Kings",
        "Holdout Clan of the Dead King's Fallen Citadel",
        "League of Babypunching Puppykickers. You feel a bit of pity for the group's unfortunate name, a poor translation from the fennecborns' native tongue for \"soft-handed littlepaw-walkers\". Maybe with your newfound sway, you can convince them to change their name",
        "monsters",
    };
    
    constexpr size_t MONSTER_COLLECTIVE = 9;
    
    constexpr const char filename[] = "savegame.txt";
        
    string playerName;
    int health = 3;
    int luck = 0;
    bool isATree = false;
    array<int, collectives.size()> influence; // Encompasses power, faith, and status
    map<string, int> items;
    vector<string> surroundings;
    
    auto AddItem = [&items](const string& key, int count)
    {
        if (items.contains(key))
        {
            items.at(key) += count;
        }
        else
        {
            items.emplace(key, count);
        }
    };
    
    auto TryRemoveItem = [&items](const string& key, int count)
    {
        auto it = items.find(key);
        if (it != items.end())
        {
            if (it->second == count)
            {
                items.erase(it);
            }
            else if (it->second > count)
            {
                it->second -= count;
            }
            return it->second >= count;
        }
        return false;
    };
    
    auto RerollSurroundings = [&surroundings]()
    {
        _RerollSurroundings(surroundings);
    };
    
    bool isResetting = true;
    
    while (isResetting)
    {
        isResetting = false;
        
        // Load the game
        {
            ifstream ifs(filename);
            if (ifs.is_open())
            {
                string pick = Prompt("Start", { "load", "new" });
                
                if (pick == "new")
                {
                    // causes ifs.is_open() to fail
                    ifs.close();
                }
            }
            else
            {
                cout << "No save found. Starting a new game.\n\n";
            }
            
            if (ifs.is_open()) // load game
            {
                size_t numItems, numSurroundings;
                getline(ifs, playerName) >> health >> luck >> isATree >> numItems >> numSurroundings;
                
                for (int& amnt : influence)
                {
                    ifs >> amnt;
                }
                
                for (size_t i = 0; i < numItems; ++i)
                {
                    string itemName;
                    int itemCount;
                    ifs >> itemName >> itemCount;
                    items.emplace(itemName, itemCount);
                }
                
                surroundings.reserve(numSurroundings);
                for (size_t i = 0; i < numSurroundings; ++i)
                {
                    string thing;
                    ifs >> thing;
                    surroundings.push_back(thing);
                }
                
                ifs.close();
                
                cout << "Welcome back, " << playerName;
            }
            else // new game
            {
                cout << "What is your name?";
                playerName = "";
                while (playerName == "")
                {
                    cout << "\n> ";
                    getline(cin, playerName);
                }
                health = 3;
                luck = 0;
                isATree = false;
                influence.fill(0);
                items = { { "gold", 5 }, { "sword", 20 } };
                RerollSurroundings();
            }
        }
        
        cout << endl;
        
        // Game loop
        while (true)
        {
            // Echo surroundings
            cout << "Your current surroundings:\n";
            for (const string& thing : surroundings)
            {
                cout << "- " << thing << '\n';
            }
            cout << "Your current inventory:\n";
            for (const auto& item : items)
            {
                cout << "- " << item.second << " " << item.first << "\n";
            }
            cout << endl;
            
            vector<string> commandOptions;
            if (isATree)
            {
                commandOptions = { "quit", "restart" };
            }
            else
            {
                commandOptions = { "move", "use", "quit", "restart" };
            }
            
            string cmd = Prompt("What would you like to do?", commandOptions);
            
            if (cmd == "move")
            {
                (void)Prompt("Where would you like to move?", { "left", "right", "forward" });
                RerollSurroundings(); // choice is an illusion :P
            }
            else if (cmd == "use")
            {
                // Request
                string action = Prompt("What would you like to use?", { "talk", "grab", "item" });
                
                if (action == "item")
                {
                    if (!items.empty())
                    {
                        action = PromptItem("Which item?", items);
                        TryRemoveItem(action, 1);
                    }
                    else
                    {
                        cout << "You don't have any items.\n";
                        continue;
                    }
                }
                
                if (action == "phonenumber")
                {
                    cout << "If only you had a phone...\n\n";
                    continue;
                }
                
                vector<string> availableTargets = surroundings;
                availableTargets.insert(availableTargets.begin() + 0, "self");
                string target = Prompt("On what?", availableTargets);
                
                string topic_or_effect;
                if (action == "talk")
                {
                    topic_or_effect = ChooseRandom(topics);
                }
                else if (action == "potion")
                {
                    if (luck > 0)
                    {
                        --luck;
                        int potionEffectsHalfSize = potionEffects.size() / 2;
                        if (target == "self") // Lower half
                        {
                            topic_or_effect = potionEffects.at(rand() % potionEffectsHalfSize);
                        }
                        else // Upper half
                        {
                            topic_or_effect = potionEffects.at(potionEffectsHalfSize + (rand() % (potionEffects.size() - potionEffectsHalfSize)));
                        }
                    }
                    else
                    {
                        topic_or_effect = ChooseRandom(potionEffects);
                    }
                }
                
                // Echo
                {
                    cout << "You ";
                    if (action == "talk")
                    {
                        cout << "had an interesting discussion regarding " << topic_or_effect << " with";
                    }
                    else if (action == "grab")
                    {
                        cout << "grabbed";
                    }
                    else if (action == "bread")
                    {
                        cout << "gave a piece of bread to";
                    }
                    else if (action == "sword")
                    {
                        cout << "swung your sword at";
                    }
                    else if (action == "potion")
                    {
                        cout << "used a potion of " << topic_or_effect << " on";
                    }
                    else if (action == "gold")
                    {
                        cout << "gave some gold to";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                    
                    cout << " ";
                    
                    if (target == "self")
                    {
                        cout << "yourself";
                    }
                    else // npc
                    {
                        cout << "the " << target;
                    }
                    cout << ".\n";
                }
                
                // Responses
                if (target == "self")
                {
                    if (action == "talk")
                    {
                        if (!surroundings.empty())
                        {
                            const array<string, 10> possibleResponses = {
                                "found it rather odd",
                                "thinks you might need a checkup",
                                "considers your points rather thought-provoking and insightful, if a little one-sided",
                                "is too distracted by your utterly repulsive views to care that you were talking to a stone wall",
                                "is confused who you were talking to",
                                "looked around to check if there were any hidden familiars you may have been putting on a performance for",
                                "gave you a weird look",
                                "thinks you underestimate the effectiveness of brute force",
                                "thinks you ",
                            };
                            
                            // It is by design that the responder might be inanimate. I thought it would be funny.
                            string responder = ChooseRandom(surroundings);
                            string response = ChooseRandom(possibleResponses);
                            if (response == "thinks you ")
                            {
                                array<string, 7> contextualOpinion = {
                                    "may have overlooked",
                                    "exaggerated",
                                    "are overestimating",
                                    "underestimate",
                                    "may forget",
                                    "fail to appriciate",
                                    "are perpetuating harmful propaganda regarding",
                                };
                                response += ChooseRandom(contextualOpinion) + ' ';
                                array<string, 3> contextualResponses;
                                if (topic_or_effect == TOPIC_WINEFISH)
                                {
                                    contextualResponses = {
                                        "the resiliance of aquatic livers",
                                        "the work ethic of drunk merfolk",
                                        "how that would affect the wine market",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_SKELESTOCK)
                                {
                                    contextualResponses = {
                                        "the staying power of good steel",
                                        "the dwindling numbers of the skeleton army",
                                        "how important armor is to the health of the Skeleton Alliance's trade sector",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_WP_SIEGE)
                                {
                                    contextualResponses = {
                                        "an average woodpecker's endurance",
                                        "the fact that a woodpecker pecks wood, not stone",
                                        "the effectiveness of hot oil on an army of woodpeckers",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_BS_TELEKEN)
                                {
                                    contextualResponses = {
                                        "the power of hydrokenesis when used to cool hot steel",
                                        "the craftsmanship needed to shape tools with one's mind",
                                        "the quality of a blade heated with telekenetic precision",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_WOODCHUCK)
                                {
                                    contextualResponses = {
                                        "where they'd even get all that wood",
                                        "how long a woodchuck's arms are",
                                        "the possibility of seven or more woodchucks banding together and forming a woodchuck mega-fusion",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_PENGUIN_BTL)
                                {
                                    contextualResponses = {
                                        "what happened the last six times the Penguin Guild and the Old Realm got into a fight like this in the past",
                                        "just how much DNA the Penguin Guild's members actually share with real penguins",
                                        "how prone the Old Realm's shadow government is to shooting themselves in the foot when it comes to matters like this",
                                    };
                                }
                                else // make up some catch-all bs
                                {
                                    contextualResponses = {
                                        "how much salt that would take",
                                        "the pulsating, glowing red mass",
                                        "the power of teamwork",
                                    };
                                }
                                response += ChooseRandom(contextualResponses);
                            }
                            cout << "The " << responder << " " << response << ".";
                        }
                        else
                        {
                            cout << (rand() & 1 ? "Fortunately" : "Sadly") << ", nobody was around to hear it...";
                        }
                    }
                    else if (action == "grab")
                    {
                        cout << "Weirdo.";
                    }
                    else if (action == "bread")
                    {
                        ++health;
                        cout << "It was delicious, replenishing 1 point of health.";
                    }
                    else if (action == "sword")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "potion")
                    {
                        if (topic_or_effect == "fire")
                        {
                            --health;
                            cout << "It hurt quite a bit, taking away 1 point of health.";
                        }
                        else if (topic_or_effect == "heal")
                        {
                            ++health;
                            cout << "You felt rejuvenated, recovering 1 point of health.";
                        }
                        else if (topic_or_effect == "water")
                        {
                            cout << "It soothes your wounds. Hopefully this doesn't rust your armor.";
                        }
                        else if (topic_or_effect == "ducks")
                        {
                            cout << "A dozen ducks pop into existence around you. They seem lost and disoriented for a moment before waddling northeast towards the nearest pond.";
                        }
                        else if (topic_or_effect == "explode")
                        {
                            health -= 3;
                            cout << "You feel your armor heat up tremendously, practically baking you within it. You feel 3 health points drain from your soul.";
                        }
                        else if (topic_or_effect == "wish")
                        {
                            cout << "A genie appears from the the potion bottle.";
                            
                            string wish = Prompt("\"What would you like for your wish, " + playerName + "?\"", {
                                "wealth", "health", "power", "status", "luck", "faith"
                            });
                            cout << "The genie pauses for a moment before replying, \"Granted.\"\n";
                            
                            if (wish == "wealth")
                            {
                                if (items.contains("gold"))
                                {
                                    items.at("gold") += 3;
                                }
                                else
                                {
                                    items.emplace("gold", 3);
                                }
                                cout << "You feel your money pouch get a little heavier.";
                            }
                            else if (wish == "health")
                            {
                                if (!surroundings.empty() && (rand() & 1))
                                {
                                    // They didn't :P
                                    cout << "\"You wished for health, you didn't specify for whom\"\nYou notice the "
                                        << ChooseRandom(surroundings) << " appears to have regained some health...";
                                }
                                else
                                {
                                    health += 2;
                                    cout << "You feel your wounds heal, replenishing 2 health points.";
                                }
                            }
                            else if (wish == "power")
                            {
                                if ((rand() & 1))
                                {
                                    size_t collectiveIndex = rand() % collectives.size();
                                    string among = collectives.at(collectiveIndex);
                                    ++influence.at(collectiveIndex);
                                    cout << "You feel an inexplicable sensation of political influence over the " << among << ".";
                                }
                                else
                                {
                                    if (items.contains("sword"))
                                    {
                                        items.at("sword") += 10;
                                    }
                                    else
                                    {
                                        items.emplace("sword", 10);
                                    }
                                    cout << "Your blade seems to magically sharpen itself, regaining 10 durability.";
                                }
                            }
                            else if (wish == "status")
                            {
                                size_t collectiveIndex = rand() % collectives.size();
                                string among = collectives.at(collectiveIndex);
                                ++influence.at(collectiveIndex);
                                cout << "You feel an inexplicable sensation of high status among the " << among << ".";
                            }
                            else if (wish == "luck")
                            {
                                luck += 5;
                                cout << "Nothing seems to have changed. You aren't even sure if the wish did anything.";
                            }
                            else if (wish == "faith")
                            {
                                size_t collectiveIndex = rand() % collectives.size();
                                string among = collectives.at(collectiveIndex);
                                ++influence.at(collectiveIndex);
                                cout << "You feel an inexplicable sensation of religious influence over the " << among << ".";
                            }
                            cout << " The genie disappears.";
                        }
                        else if (topic_or_effect == "demon")
                        {
                            array<string, 4> demonTypes = {
                                "imp",
                                "warewolf",
                                "tentacle",
                                "vampire",
                            };
                            string demon = ChooseRandom(demonTypes);
                            cout << "You are surprised by the sudden appearance of " << (isvowel(demon[0]) ? "an" : "a") << " " << demon << ". ";
                            
                            if (demon == "imp")
                            {
                                health -= 2;
                                cout << "The imp seems rather annoyed, throwing a fireball at you before de-summoning itself. You take 2 points of damage.";
                            }
                            else if (demon == "warewolf")
                            {
                                if (luck > 0)
                                {
                                    --luck;
                                    cout << "The warewolf seems friendly, and you get a chance to play fetch with it before it disappears in a puff of smoke.";
                                }
                                else
                                {
                                    health -= 2;
                                    cout << "The warewolf growls at you and bites your arm before disappearing in a puff of smoke. You are protected from most of the attack thanks to your armor, but it still takes away 2 points of your health.";
                                }
                            }
                            else if (demon == "tentacle")
                            {
                                --health;
                                cout << "The tentacle slaps you violently, causing you to spin around in place before it slithers back down into its hole, disappearing from view. You lost 1 point of health.";
                            }
                            else if (demon == "vampire")
                            {
                                if (luck > 0)
                                {
                                    --luck;
                                    AddItem("bread", 3);
                                    cout << "The vampire seemed confused as to why they were summoned, but decides now is as decent a time as any to dispose of the garlic bread people keep throwing at them. They " << ChooseRandom({ "plop", "stuff", "drop", "place" }) << " it down into your hand and promptly transform into a small black bat, flying away into the darkness.";
                                }
                                else
                                {
                                    cout << "The vampire seems frustrated by your frivolous summoning and kicks you in your armored shin before transforming into a bat and gliding off into the night. You take no damage, but it still stings.";
                                }
                            }
                        }
                        else if (topic_or_effect == "salt")
                        {
                            cout << "Your wounds begin burning and you feel dehydrated.";
                        }
                        else if (topic_or_effect == "tree")
                        {
                            cout << "You transform into a tree.";
                        }
                        else if (topic_or_effect == "ants")
                        {
                            cout << "[TODO]\n";
                        }
                        else if (topic_or_effect == "force")
                        {
                            cout << "[TODO]\n";
                        }
                        else if (topic_or_effect == "predict")
                        {
                            luck += 5;
                            cout << "You recieve foresight of everything that will happen in the near future, and plan accordingly.";
                        }
                    }
                    else if (action == "gold")
                    {
                        AddItem("gold", 1);
                        cout << "You make a show of thanking yourself kindly for the gold before returning it to your money pouch.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "door")
                {
                    if (action == "talk")
                    {
                        cout << "The door listens silently.";
                    }
                    else if (action == "grab")
                    {
                        RerollSurroundings();
                        cout << "The door opens, allowing you through.";
                    }
                    else if (action == "bread")
                    {
                        ++luck;
                        cout << "The door says nothing, but you get the odd sensation that it appriciates the gift.";
                    }
                    else if (action == "sword")
                    {
                        if (rand() & 3) // 3/4 chance - 01, 10, 11 = true; 00 = false
                        {
                            cout << "The door does not yield.";
                        }
                        else
                        {
                            RerollSurroundings();
                            cout << "The door crumbles to bits and you walk through it.";
                        }
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        cout << "The door remains silent and unmoved. It has no concept for money.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "baker")
                {
                    if (action == "talk")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "grab")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "bread")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "sword")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        AddItem("bread", 1);
                        cout << "The baker thanks you for the gold and hands you a piece of bread.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "smith")
                {
                    if (action == "talk")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "grab")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "bread")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "sword")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        AddItem("sword", 10);
                        cout << "The blacksmith thanks you for the gold and adds some durability to your sword.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "wizard")
                {
                    if (action == "talk")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "grab")
                    {
                        bool isPositive = rand() & 1;
                        string reaction, actionDescriptor;
                        if (isPositive)
                        {
                            reaction = ChooseRandom({ "intrigued", "embarrassed" });
                            actionDescriptor = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
                        }
                        else
                        {
                            reaction = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
                            actionDescriptor = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
                        }
                        cout << "The wizard appears " << reaction << " by your " << actionDescriptor << " action.";
                    }
                    else if (action == "bread")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "sword")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        AddItem("potion", 1);
                        cout << "The wizard thanks you for the gold and gives you a potion.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "monster")
                {
                    bool isPositive = (rand() % 20) < influence.at(MONSTER_COLLECTIVE);
                    
                    // Only uses luck if you wouldn't have succeeded and weren't trying to hurt it
                    if (!isPositive && luck > 0 && action != "sword" && action != "potion")
                    {
                        --luck;
                        isPositive = true;
                    }
                    
                    if (action == "talk")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "grab")
                    {
                        string reaction, reactingTo;
                        if (isPositive)
                        {
                            reaction = ChooseRandom({ "surprised", "impressed", "confused", "baffled", "amused", "astonished", "flustered", "bewildered" });
                            reactingTo = ChooseRandom({ "forwardness", "strength", "agility", "fearlessness", "courage" });
                        }
                        else
                        {
                            reaction = ChooseRandom({ "surprised", "confused", "unamused", "annoyed", "upset", "angered" });
                            reactingTo = ChooseRandom({ "foolishness", "lack of self-preservation", "disrespect" });
                        }
                        
                        cout << "The monster seems " << reaction << " by your " << reactingTo << ", staring at you until you release it from your grip.\n";
                        
                        if (reaction == "flustered" || ((reaction == "impressed" || reaction == "surprised") && reactingTo == "forwardness"))
                        {
                            AddItem("phonenumber", 1);
                            cout << "Upon being released, it discretely slips something into your pocket.";
                        }
                        else if (isPositive)
                        {
                            ++influence.at(MONSTER_COLLECTIVE);
                            cout << "It gives a little nod of respect.";
                        }
                        else // negative
                        {
                            health -= 3;
                            cout << "It decides to return the favor, squeezing you in its massive claws as you feel your armor dent and tighten around your fragile, squishy body. ";
                            cout << "You lose 3 points of health.";
                        }
                    }
                    else if (action == "bread")
                    {
                        cout << "The monster ";
                        if (isPositive)
                        {
                            string reaction = ChooseRandom({
                                "is delighted by",
                                "is overjoyed at",
                                "looks longingly at",
                                "appriciates",
                                "tears up over",
                                "stares in surprise at",
                                "curiously examines",
                                "curiously investigates",
                            });
                            string response = ChooseRandom({
                                "scarfing it down hungrily",
                                "taking long, deliberate bites out of it",
                                "tucking it away for later",
                                "sheathing it on its back like a sword",
                                "sliding the entire loaf down its neck in one bite",
                                "slipping the loaf into its conveniently-sized hat",
                                "stuffing the loaf through what you had previously mistaken for a cycloptic eye",
                                "letting the bread melt and assimilate into its smooth, featureless face",
                            });
                            string gratitude = ChooseRandom({
                                "nodding its head with what is meant to be a genuine smile, though it has difficulty trying not to look frightening",
                                "giving you what is meant to be a pat on the head; unintentionally causing your helmet to clang against the neck of your breastplate",
                                "wrapping its long, heavy arms around you in thanks. It drops you onto the floor after a few seconds, and you lie there stunned for a moment before managing to stand back up",
                                "pulling you into a warm bear hug for a full thirty seconds before setting you down",
                            });
                            ++influence.at(MONSTER_COLLECTIVE);
                            cout << reaction << " the gift, quietly accepting it and " << response << " before " << gratitude << ".";
                        }
                        else
                        {
                            string reaction = ChooseRandom({ "scowels", "scoffs", "screams", "screeches", "foams", "growls", "hisses", });
                            cout << reaction << " at the very idea of accepting a gift from you, slapping it out of your hand and onto the floor.";
                        }
                    }
                    else if (action == "sword")
                    {
                        bool isSuccessful = rand() & 1; // 50% chance
                        if (!isSuccessful && luck > 0)
                        {
                            --luck;
                            isSuccessful = true;
                        }
                        
                        if (isSuccessful)
                        {
                            string bodyCovering = ChooseRandom({ "gooey", "vaporous", "slimey", "meaty", "fuzzy", "fur-covered" });
                            cout << "Your sword slashes through the monster, splitting it into two " << bodyCovering << " chunks on the floor. ";
                            AddItem("gold", 1);
                            cout << "A gold coin drops to the cold, hard floor with a clink. You pick it up, adding it to your collection as the body fizzles to dust.";
                        }
                        else
                        {
                            TryRemoveItem("sword", 1);
                            bool isSwordSurviving = items.contains("sword");
                            
                            string reaction = ChooseRandom({
                                "grabs your sword mid-swing, gripping it tightly in its claws",
                                "swats your sword out of your hand, flinging it into the wall",
                                "squeezes your sword's blade with two hands",
                                "bites down on your sword violently with its massive shark-teeth",
                            });
                            
                            string impact;
                            if (isSwordSurviving)
                            {
                                // adverb
                                impact = ChooseRandom({ "slightly", "somewhat", "lightly", "partly", "gently", });
                                // verb
                                impact = " " + ChooseRandom({ "bending", "dulling", "denting", "melting", "cracking", "chipping", });
                                impact += " it";
                            }
                            else
                            {
                                // adverb
                                impact = ChooseRandom({ "completely", "totally", "fully", "absolutely", "utterly", "instantly", });
                                // verb
                                impact += " " + ChooseRandom({ "shattering", "obliterating", "liquifying", "anihilating", "evaporating", "disintegrating",
                                    "digesting", // Yes, even for non-mouth reactions
                                });
                                impact += " everything above the hilt";
                            }
                            
                            cout << "The monster " << reaction << " and " << impact << '.';
                        }
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        cout << "The monster doesn't know what to do with the gold, tilting its head curiously but quickly losing interest.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else
                {
                    throw new NotImplementedException();
                }
                
                cout << endl;
            }
            else if (cmd == "quit")
            {
                isResetting = false;
                break;
            }
            else if (cmd == "restart")
            {
                isResetting = true;
                break;
            }
            
            cout << endl;
            
            if (health <= 0)
            {
                cout << "Your health has dropped to zero and you have died.\n";
                string tryAgain = Prompt("Would you like to start again?", { "yes", "no" });
                if (tryAgain == "yes")
                {
                    isResetting = true;
                    break;
                }
            }
        }
    }
    
    // Save the game
    {
        ofstream ofs(filename, ofstream::trunc);
        ofs << playerName << '\n';
        ofs << health << ' ' << luck << ' ' << isATree << ' ' << items.size() << ' ' << surroundings.size();
        
        for (int& amnt : influence)
        {
            ofs << ' ' << amnt;
        }
        ofs << '\n';
        
        for (const auto& item : items)
        {
            ofs << item.first << ' ' << item.second << '\n';
        }
        
        for (const string& thing : surroundings)
        {
            ofs << thing << '\n';
        }
        
        ofs.close();
    }
    
    return 0;
}

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

string PromptItem(const string& prompt, const map<string, int>& options)
{
    cout << prompt;
    for (const auto& it : options)
    {
        cout << "\n- " << it.first << " : " << it.second;
    }
    cout << endl;
    while (true) // repeats until return
    {
        string input;
        cout << "> ";
        getline(cin, input);
        auto it = options.find(input);
        if (it != options.end())
        {
            return it->first;
        }
    }
}

void _RerollSurroundings(vector<string>& surroundings)
{
    surroundings.clear();
    
    const array<string, 5> possible = {
        "door",    // Do nothing :P
        "baker",   // Give bread
        "smith",   // Repair sword
        "wizard",  // Give potion
        "monster", // Hurt the player
    };
    
    for (int i = 0; i < 4; ++i) // Four possible items per roll
    {
        if (rand() & 1) // Whether to *try* adding something to surroundings (not guaranteed)
        {
            string choice = ChooseRandom(possible);
            
            // Duplicates are not added
            if (find(surroundings.begin(), surroundings.end(), choice) == surroundings.end())
            {
                surroundings.push_back(choice);
            }
        }
    }
}
