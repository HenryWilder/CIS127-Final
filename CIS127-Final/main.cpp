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
    string test = "item 1, item 2, item 3, item 4";
    split(test, ", ", [](const string_view& view)
    {
        cout << view << '\n';
        return true;
    });

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
