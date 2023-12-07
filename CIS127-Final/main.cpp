#include "utilities.hpp"
#include "TurnEchoStream.hpp"
#include "serialization.hpp"
#include "TurnHandler.hpp"
#include "ExtendedAscii.hpp"

#pragma region Instructions
constexpr const wchar_t *instructions = LR"(
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

void ConfigureUnicode( );

int main( )
{
    ConfigureUnicode( );

    cout << endl;

    while (turnHandler.GetTurnEndType( ) != TurnEndType::Quit)
    {
        // Load the game
        LoadWithPrompt( );

        cout << instructions << endl;

        echo << '\n'; // To put some space between the "[]" messages and this
        turnHandler.DoNear( );
        echo << '\n';
        turnHandler.DoItems( );
        FlushEcho( );
        cout << endl;

        // Game loop
        while (turnHandler.GetTurnEndType( ) == TurnEndType::Continue)
        {
            if (!turnHandler.CheckPlayerIsAlive( )) break;

            turnHandler.DoAction(turnHandler.PromptForAction( ));

            FlushEcho( );

            surroundings.ReleaseQueued( );

            cout << endl;
        }
    }

    // Save the game
    Save( );

    return 0;
}

void ConfigureUnicode( )
{
    cout << "Some consoles have trouble displaying unicode characters.\n" << endl;

    // ░ ▒ ▓ │ ┤ ╡ ╢ ╖
    // ╕ ╣ ║ ╗ ╝ ╜ ╛ ┐
    // └ ┴ ┬ ├ ─ ┼ ╞ ╟
    // ╚ ╔ ╩ ╦ ╠ ═ ╬ ╧
    // ╨ ╤ ╥ ╙ ╘ ╒ ╓ ╫
    // ╪ ┘ ┌ █ ▄ ▌ ▐ ▀

    const wchar_t example[ ] =
        L" ┌───────────────┐ \n"
        L" │ Hello, world! │ \n"
        L" └───────────────┘ \n"
        L" ╔══════════════════════╤═══╕ \n"
        L" ▐ This is an example.  │ 1.│\n"
        L" ╟──────────────────────┼───┴──╖ ╓──╖\n"
        L" ║    ░░░░░▒▒▒▒▓▓▓▓████ │ 2. █ ╠═╬══╣\n"
        L" ╚══════════════════════╧══════╝ ╙──╜\n";

    SetUseUnicode(false);
    cout << " ascii" << '\n' << example << endl;
    SetUseUnicode(true);
    cout << " unicode" << '\n' << example << endl;

    string userPick = PromptOption("Which of the above do you prefer?", { "unicode", "ascii" });
    SetUseUnicode(userPick == "unicode");
}
