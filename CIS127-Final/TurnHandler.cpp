#include "utilities.hpp"
#include "TurnHandler.hpp"

TurnHandler turnHandler;

Action TurnHandler::PromptForAction()
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
        if (!player.IsInventoryEmpty())
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

pair<EntityTypeInfo_t, Entity&> TurnHandler::PromptForTartget(const string& prompt)
{
    EntityTypeInfo_t target = entityTypes.At(surroundings.Prompt(prompt));
    Entity& targetObject = surroundings.Get(target);
    return { target, targetObject };
}

void TurnHandler::DoMove()
{
    Direction direction = directions.Prompt("\nWhere would you like to move?");
    surroundings.ReRoll(); // choice is an illusion :P

    echo << format("You walked {} into another room.\n\n", directions.ValueAt(direction));

    if (!surroundings.IsEmpty())
    {
        constexpr ListStyle sightListStyle = { "You see ", "a ", "", "", ", ", " and ", ".", false };
        StreamList::Push(sightListStyle);
        surroundings.Print(echo);
        StreamList::Pop();
    }
    else
    {
        echo << "You are alone with your thoughts.";
    }
}

void TurnHandler::DoTalk()
{
    auto [target, targetObject] = PromptForTartget("\nWho/what would you like to talk to?");

    TopicInfo_t topic = topics.Random();

    echo << format("You had an interesting discussion with {} regarding {}.\n\n", target.full, topic.full);

    targetObject.DoInteraction_Talk(topic);
}

void TurnHandler::DoGrab()
{
    auto [target, targetObject] = PromptForTartget("\nWho/what would you like to grab?");

    echo << format("You grabbed {}.\n\n", target.full);

    targetObject.DoInteraction_Grab();
}

void TurnHandler::DoUseItem(Item item, EntityTypeInfo_t target, Entity& targetObject)
{
    switch (item)
    {
    case Item::Bread:
    {
        echo << format("You gave a piece of bread to {}.\n\n", target.full);
        return targetObject.DoInteraction_Bread();
    }

    case Item::Sword:
    {
        echo << format("You swung your sword at {}.\n\n", target.full);
        return targetObject.DoInteraction_Sword();
    }

    case Item::Gold:
    {
        echo << format("You gave some gold to {}.\n\n", target.full);
        return targetObject.DoInteraction_Gold();
    }

    case Item::Potion:
    {
        PotionInfo_t potion = potions.Random();
        echo << format("You used a potion of {} on {}.\n\n", potion.full, target.full);
        return targetObject.DoInteraction_Potion(potion);
    }

    case Item::Phonenumber:
    {
        // todo
        return;
    }
    }
    throw new NotImplementedException(items.KeyAt(item));
}

void TurnHandler::DoUse()
{
    Item item;
    item = player.PromptItem("\nWhich item would you like to use?");

    if (item == Item::Phonenumber)
    {
        echo << "If only you had a phone...";
        return;
    }

    player.TryRemoveItemQty(item, 1);

    string prompt = format("\nWho/what would you like to use your {} on?", items.ValueAt(item));
    auto [target, targetObject] = PromptForTartget(prompt);

    DoUseItem(item, target, targetObject);
}

void TurnHandler::DoItems()
{
    StreamList::Push(StreamList::JSONObjectList);

    echo << "Your current inventory: ";
    player.PrintInventory(echo);

    StreamList::Pop();
}

void TurnHandler::DoNear()
{
    StreamList::Push(StreamList::JSONValueList);

    echo << "Your current surroundings: ";
    surroundings.Print(echo);

    StreamList::Pop();
}

void TurnHandler::DoQuit()
{
    turnEndType = TurnEndType::Quit;
}

void TurnHandler::DoRestart()
{
    turnEndType = TurnEndType::Restart;
}

void TurnHandler::DoAction(Action action)
{
    switch (action)
    {
    case Action::Move:    return DoMove();
    case Action::Talk:    return DoTalk();
    case Action::Grab:    return DoGrab();
    case Action::Use:     return DoUse();
    case Action::Items:   return DoItems();
    case Action::Near:    return DoNear();
    case Action::Quit:    return DoQuit();
    case Action::Restart: return DoRestart();
    }
    throw new NotImplementedException(actions.KeyAt(action));
}

bool TurnHandler::CheckPlayerIsAlive()
{
    if (player.IsDead())
    {
        echo << "Your health has dropped to zero and you have died.";
        FlushEcho();
        bool wantsToRestart = (bool)boolean.Prompt("Would you like to start again?");
        turnEndType = wantsToRestart ? TurnEndType::Restart : TurnEndType::Quit;
        return false;
    }
    return true;
}
