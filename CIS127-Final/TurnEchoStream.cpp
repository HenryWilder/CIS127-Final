#include "utilities.hpp"
#include "TurnEchoStream.hpp"
#include "ExtendedAscii.hpp"

stringstream echo;

void FlushEcho()
{
    constexpr char BOX_HORIZONTAL_LINE_CHAR = '\xC4'; // L'─'

    // Don't print blank

    echo.seekg(0, ios::end);
    if (echo.tellg() == 0) return;

    // Print

    echo.seekg(0, ios::beg);

    cout << L"\n ┌─" << setfill(BOX_HORIZONTAL_LINE_CHAR) << setw(echoBoxInsideWidth) << "" << L"─┐\n";

    string line;
    while (getline(echo, line))
    {
        // Line is blank before parsing
        if (line.empty())
        {
            cout << L" │ " << setfill(' ') << setw(echoBoxInsideWidth) << "" << L" │\n";
        }

        // Line is consumed as it wraps over multiple lines
        while (!line.empty())
        {
            bool isWiderThanBox = line.size( ) > echoBoxInsideWidth;
            string linePart = line.substr(0, echoBoxInsideWidth);

            // Needs to wrap
            if (isWiderThanBox)
            {
                // Wrap at sentence break
                if (size_t lastPeriod = linePart.rfind(". "); lastPeriod != string::npos)
                {
                    linePart.erase(lastPeriod + 2);
                    line.erase(0, lastPeriod + 2);
                }
                // Wrap at word break
                else if (size_t lastSpace = linePart.rfind(' '); lastSpace != string::npos)
                {
                    linePart.erase(lastSpace + 1);
                    line.erase(0, lastSpace + 1);
                }
                // Wrap at character
                else
                {
                    line.erase(0, echoBoxInsideWidth);
                }
            }
            // No wrapping
            else
            {
                line.erase(0, echoBoxInsideWidth);
            }

            cout << L" │ " << setfill(' ') << left << setw(echoBoxInsideWidth) << linePart << L" │\n";
        }
    }

    cout << L" └─" << setfill(BOX_HORIZONTAL_LINE_CHAR) << setw(echoBoxInsideWidth) << "" << L"─┘\n";

    // Empty the echo stream so we don't repeat it next flush
    echo.str(string());
    echo.clear();
}
