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

enum class TurnEndType
{
    Continue,
    Quit,
    Restart,
};

class TurnHandler
{
private:
    static bool IsPlayerSoftLocked()
    {
        return player.health.statuses.Has(StatusEffects::Tree);
    }

    static bool IsPlayerWithoutItems()
    {
        return player.health.statuses.Has(StatusEffects::Tree);
    }

public:
    TurnEndType GetTurnEndType() const
    {
        return turnEndType;
    }

    static Action PromptForAction()
    {
        const array<Action, 2> alwaysAvailable = { Action::Quit, Action::Restart };

        if (!IsPlayerSoftLocked())
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
            return PromptOptionWithHidden("What would you like to do?", options, alwaysAvailable);
        }
        else // Soft locked
        {
            return PromptOption("You are a tree.", alwaysAvailable);
        }
    }

    static pair<EntityTypeInfo_t, Entity&> PromptForTartget(const string& prompt)
    {
        EntityTypeInfo_t target = entityTypes.At(surroundings.Prompt(prompt));
        Entity& targetObject = surroundings.Get(target);
        return { target, targetObject };
    }

    static void BeginTurnEcho()
    {
        HorizontalRule();
        cout << '\n';
    }
    static void EndTurnEcho()
    {
        cout << '\n';
        HorizontalRule();
    }

    void DoMove()
    {
        Direction direction = directions.Prompt("Where would you like to move?");
        surroundings.ReRoll(); // choice is an illusion :P

        BeginTurnEcho();

        cout << format("You walked {} into another room.\n", directions.ValueAt(direction));

        if (!surroundings.IsEmpty())
        {
            constexpr ListStyle sightListStyle = { "You see ", "a ", "", "", ", ", " and ", ".", false };
            StreamList::Push(sightListStyle);
            surroundings.Print();
            StreamList::Pop();
        }
        else
        {
            cout << "You are alone with your thoughts.";
        }
        
        EndTurnEcho();
    }

    void DoTalk()
    {
        auto [target, targetObject] = PromptForTartget("Who/what would you like to talk to?");

        TopicInfo_t topic = topics.Random();

        BeginTurnEcho();

        cout << format("You had an interesting discussion with {} regarding {}.\n", target.full, topic.full);

        targetObject.DoInteraction_Talk(topic);

        EndTurnEcho();
    }

    void DoGrab()
    {
        auto [target, targetObject] = PromptForTartget("Who/what would you like to grab?");

        BeginTurnEcho();

        cout << format("You grabbed {}.\n", target.full);

        targetObject.DoInteraction_Grab();

        EndTurnEcho();
    }

    static void DoUseItem(Item item, EntityTypeInfo_t target, Entity& targetObject)
    {
        switch (item)
        {
        case Item::Bread:
        {
            cout << format("You gave a piece of bread to {}.\n", target.full);
            return targetObject.DoInteraction_Bread();
        }

        case Item::Sword:
        {
            cout << format("You swung your sword at {}.\n", target.full);
            return targetObject.DoInteraction_Sword();
        }

        case Item::Gold:
        {
            cout << format("You gave some gold to {}.\n", target.full);
            return targetObject.DoInteraction_Gold();
        }

        case Item::Potion:
        {
            PotionInfo_t potion = potions.Random();
            cout << format("You used a potion of {} on {}.\n", potion.full, target.full);
            return targetObject.DoInteraction_Potion(potion);
        }
        }
        throw new NotImplementedException(items.KeyAt(item));
    }

    static void DoUse()
    {
        Item item;
        item = player.inventory.Prompt("Which item would you like to use?");

        if (item == Item::Phonenumber)
        {
            cout << "If only you had a phone...\n";
            return;
        }

        player.inventory.TryRemove(item, 1);

        string prompt = format("Who/what would you like to use your {} on?", items.ValueAt(item));
        auto [target, targetObject] = PromptForTartget(prompt);

        BeginTurnEcho();

        DoUseItem(item, target, targetObject);

        EndTurnEcho();
    }

    void DoItems()
    {
        StreamList::Push(StreamList::JSONObjectList);

        BeginTurnEcho();

        cout << "Your current inventory:\n";
        player.inventory.Print();

        EndTurnEcho();

        StreamList::Pop();
    }

    void DoNear()
    {
        StreamList::Push(StreamList::JSONValueList);

        BeginTurnEcho();

        cout << "Your current surroundings:\n";
        surroundings.Print();

        EndTurnEcho();

        StreamList::Pop();
    }

    void DoQuit()
    {
        turnEndType = TurnEndType::Quit;
    }

    void DoRestart()
    {
        turnEndType = TurnEndType::Restart;
    }

    void DoAction(Action action)
    {
        switch (action)
        {
        case Action::Move:    return DoMove   ();
        case Action::Talk:    return DoTalk   ();
        case Action::Grab:    return DoGrab   ();
        case Action::Use:     return DoUse    ();
        case Action::Items:   return DoItems  ();
        case Action::Near:    return DoNear   ();
        case Action::Quit:    return DoQuit   ();
        case Action::Restart: return DoRestart();
        }
        throw new NotImplementedException(actions.KeyAt(action));
    }

    // Returns false if dead
    bool CheckPlayerIsAlive()
    {
        if (player.health.IsDead())
        {
            cout << "Your health has dropped to zero and you have died.\n";
            bool wantsToRestart = (bool)boolean.Prompt("Would you like to start again?");
            turnEndType = wantsToRestart ? TurnEndType::Restart : TurnEndType::Quit;
            return false;
        }
        return true;
    }

private:
    TurnEndType turnEndType = TurnEndType::Continue;
} turnHandler;

int main()
{    
    while (turnHandler.GetTurnEndType() != TurnEndType::Quit)
    {
        // Load the game
        LoadWithPrompt();
        
        PrintInstructions();

        // Game loop
        while (turnHandler.GetTurnEndType() == TurnEndType::Continue)
        {
            if (!turnHandler.CheckPlayerIsAlive())
            {
                break;
            }
            
            turnHandler.DoAction(turnHandler.PromptForAction());

            surroundings.ReleaseQueued();

            cout << endl;
        }
    }
    
    // Save the game
    Save();
    
    return 0;
}
