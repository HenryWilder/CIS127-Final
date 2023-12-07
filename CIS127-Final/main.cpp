#include "utilities.hpp"
#include "TurnEchoStream.hpp"
#include "serialization.hpp"
#include "TurnHandler.hpp"
#include "ExtendedAscii.hpp"
#include <string_view>

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

int main(int argc, const char *argv[])
{
    bool isShowingInstructions = true;
    bool isPromptingDisplayMode = true;
    const char* playerName = nullptr;
    AutoLoadOrNew autoLoadOrNew = AutoLoadOrNew::Ask;

    for (int i = 1; i < argc; ++i)
    {
        std::string_view arg = argv[i];

        // Unicode output
        if (arg == "-u" || arg == "--unicode")
        {
            SetUseUnicode(true);
            isPromptingDisplayMode = false;
        }

        // ASCII output
        else if (arg == "-a" || arg == "--ascii")
        {
            SetUseUnicode(false);
            isPromptingDisplayMode = false;
        }

        // Suppress instructions
        else if (arg == "-s" || arg == "--no-instructions")
        {
            isShowingInstructions = false;
        }

        // Start new game
        else if (arg == "-n" || arg == "--new")
        {
            autoLoadOrNew = AutoLoadOrNew::New;

            if (++i < argc)
            {
                playerName = argv[i];
            }
            else
            {
                cout << "Error: Missing command-line argument.\n";
                cout << "Expected a name argument following '" << arg << "'.\n" << endl;
                return 1;
            }
        }

        // Try to load an existing save
        else if (arg == "-l" || arg == "--load")
        {
            autoLoadOrNew = AutoLoadOrNew::Load;
        }

        // Help
        else if (arg == "--help")
        {
            cout << "Usage: " << argv[0] << " [options]\n"
                << '\n'
                << "Options:\n"
                << "  -u or --unicode            - Enable Unicode display (instead of ascii).\n"
                << "  -a or --ascii              - Enable ASCII display (instead of unicode).\n"
                << "  -s or --no-instructions    - Suppress the instructions.\n"
                << "  -n <name> or --new <name>  - Start a new game named <name>.\n"
                << "  -l or --load               - Load the existing save (fails if no save is found).\n"
                << "  --help                     - Show this help menu.\n" << endl;

            return 0;
        }

        // Unknown option
        else
        {
            cout << "Error: Unknown command-line argument.\n";
            cout << "Argument " << i << ", '" << arg << "', is not a known option. Use --help for a list of valid options.\n" << endl;
            return 1;
        }
    }

    if (isPromptingDisplayMode)
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
        cout << endl;
    }

    while (turnHandler.GetTurnEndType( ) != TurnEndType::Quit)
    {
        // Load the game
        LoadWithPrompt(autoLoadOrNew, playerName);

        if (isShowingInstructions)
        {
            cout << instructions << endl;
            echo << '\n'; // To put some space between the "[]" messages and this
        }

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
