#include "utilities.hpp"
#include "TurnEchoStream.hpp"
#include "serialization.hpp"
#include "TurnHandler.hpp"
#include "ExtendedAscii.hpp"
#include "SerializationUtils.hpp"

#pragma region Instructions
constexpr const wchar_t* instructions = LR"(
 ┌─[ Methods of input ]─────────────────────────────────┐
 │                                                      │
 │ ┌─[ Single recognized word ]──────────────┐          │
 │ │                                         │          │
 │ ▐ Prompts with a list of recognized words │          │
 │ │                                         │          │
 │ │ ┌─[ Examples ]─┐                        │          │
 │ │ │ > use        │                        │          │
 │ │ │ > bread      │                        │          │
 │ │ │ > myself     │                        │          │
 │ │ ╞══════════════╡                        │          │
 │ │ │ > grab       │                        │          │
 │ │ │ > door       │                        │          │
 │ │ ╞══════════════╡                        │          │
 │ │ │ > move       │                        │          │
 │ │ │ > left       │                        │          │
 │ │ └──────────────┘                        │          │
 │ └─────────────────────────────────────────┘          │
 ╞══════════════════════════════════════════════════════╡
 │                                                      │
 │ ┌─[ Simple sentence with recognized words ]────────┐ │
 │ │                                                  │ │
 │ ▐ Unrecognized words are ignored                   │ │
 │ │                                                  │ │
 │ │ ┌─[ Examples ]─────────────────────────────────┐ │ │
 │ │ │ > use some of the tasty bread to feed myself │ │ │
 │ │ ╞══════════════════════════════════════════════╡ │ │
 │ │ │ > quickly grab the sturdy door and open it   │ │ │
 │ │ ╞══════════════════════════════════════════════╡ │ │
 │ │ │ > move through the door on my left           │ │ │
 │ │ └──────────────────────────────────────────────┘ │ │
 │ └──────────────────────────────────────────────────┘ │
 ╞══════════════════════════════════════════════════════╡
 │                                                      │
 ▐ You can type 'quit' or 'restart' at any time.        │
 │                                                      │
 └──────────────────────────────────────────────────────┘
)";
#pragma endregion

int main()
{
    stringstream stream;

    cout << "Writing...\n\n";

    // Test save
    {
        Writer testWriter(stream);
        testWriter.Write("x", 5);
        testWriter.Write("y", 4564);
        testWriter.Write("apple", "orange");
        testWriter.Write("balana", "This item is erroneous and should except.");

        {
            const char* listItems[] = {
                "\tString properties are treated as newline-terminated \"raw\" strings. ",
                " The entire line is read, and the quote marks are to allow for leading/trailing whitespace.  "
            };
            ScopeID listId = testWriter.BeginList("list");
            for (const char* item : listItems)
            {
                testWriter.Write(item);
            }
            testWriter.EndList(listId);
        }

        {
            ScopeID objId = testWriter.BeginObject("object");
            testWriter.Write("0", "This is my first property :)");
            testWriter.Write("intensity", 5);
            testWriter.Write("item", Item::Bread);
            {
                ScopeID objListId = testWriter.BeginList("listProperty");
                for (size_t i = 0; i < 10; ++i)
                {
                    testWriter.Write(i * 294);
                }
                testWriter.EndList(objListId);
            }
            {
                ScopeID objObjId = testWriter.BeginObject("objectProperty");
                testWriter.Write("apple", Action::Grab);
                ScopeID objObjListId = testWriter.BeginObject("objectListProperty");
                for (size_t i = 0; i < 20; ++i)
                {
                    testWriter.Write((float)i * 453.45f);
                }
                testWriter.EndObject(objObjListId);
                testWriter.EndObject(objObjId);
            }
            testWriter.EndObject(objId);
        }
    }

    //stream.seekg(0, ios::beg);
    cout << "Written:\n";
    cout << setfill('-') << setw(10) << "" << '\n';
    cout << stream.str() << '\n';
    cout << setfill('-') << setw(10) << "" << '\n' << endl;

    cout << "Reading...\n\n";

    cout << "Read:\n";
    cout << setfill('-') << setw(10) << "" << '\n';

    // Test load
    {
        Reader testReader(stream);
        int x, y;
        string apple;

        testReader.Read("x", x);
        cout << "x = " << x << '\n';

        testReader.Read("y", y);
        cout << "y = " << y << '\n';

        testReader.Read("apple", apple);
        cout << "apple = \"" << apple << "\"\n";

        try
        {
            string nonExistent;
            cout << "banana = ";
            testReader.Read("banana", nonExistent);
        }
        catch (runtime_error* err)
        {
            cout << "Intentional exception:  " << err->what() << "\n";
        }

        {
            ScopeID listId = testReader.BeginList("list");
            for (size_t i = 0; !testReader.IsEndOfList(listId); ++i)
            {
                string item;
                testReader.Read(item);
                cout << "list[" << i << "] = \"" << item << "\"\n";
            }
        }

        {
            ScopeID objId = testReader.BeginObject("object");

            string firstProp;
            testReader.Read("0", firstProp);
            cout << "object.firstProp = " << firstProp << '\n';

            int intensity;
            testReader.Read("intensity", intensity);
            cout << "object.intensity = " << intensity << '\n';

            Item item;
            testReader.Read("item", item);
            cout << "object.item = " << item << '\n';

            {
                ScopeID listPropertyId = testReader.BeginList("listProperty");
                for (size_t i = 0; !testReader.IsEndOfList(listPropertyId); ++i)
                {
                    int listItem;
                    testReader.Read(listItem);
                    cout << "object.listProperty[" << i << "] = " << listItem << "\n";
                }
            }

            {
                ScopeID objPropertyId = testReader.BeginObject("objectProperty");
                Action apple;
                testReader.Read("apple", apple);
                cout << "object.objectProperty.apple = " << apple << '\n';
                {
                    ScopeID objListPropertyId = testReader.BeginList("objectListProperty");
                    for (size_t i = 0; !testReader.IsEndOfList(objListPropertyId); ++i)
                    {
                        float amount;
                        testReader.Read(amount);
                        cout << "object.objectProperty.objectListProperty[" << i << "] = " << amount << "\n";
                    }
                }
                testReader.EndObject(objPropertyId);
            }

            testReader.EndObject(objId);
        }
    }

    cout << setfill('-') << setw(10) << "" << '\n' << endl;

    return 0;

    while (turnHandler.GetTurnEndType() != TurnEndType::Quit)
    {
        // Load the game
        LoadWithPrompt();

        cout << instructions << endl;

        turnHandler.DoNear();
        echo << '\n';
        turnHandler.DoItems();
        FlushEcho();
        cout << endl;

        // Game loop
        while (turnHandler.GetTurnEndType() == TurnEndType::Continue)
        {
            if (!turnHandler.CheckPlayerIsAlive())
            {
                break;
            }
            
            turnHandler.DoAction(turnHandler.PromptForAction());

            FlushEcho();

            surroundings.ReleaseQueued();

            cout << endl;
        }
    }
    
    // Save the game
    Save();
    
    return 0;
}
