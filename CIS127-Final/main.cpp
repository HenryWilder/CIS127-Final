#include "utilities.hpp"
#include "TurnEchoStream.hpp"
#include "serialization.hpp"
#include "TurnHandler.hpp"

void PrintInstructions();

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

            FlushEcho();

            surroundings.ReleaseQueued();

            cout << endl;
        }
    }
    
    // Save the game
    Save();
    
    return 0;
}

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
