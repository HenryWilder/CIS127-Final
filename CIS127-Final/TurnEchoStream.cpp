#include "utilities.hpp"
#include "TurnEchoStream.hpp"
#include "ExtendedAscii.hpp"

stringstream echo;

void Echo_BlankLine()
{
    cout << L" │ ";
    for (size_t i = 0; i < echoBoxInsideWidth; ++i)
    {
        cout << ' ';
    }
    cout << L" │\n";
}

void FlushEcho()
{
    // Don't print blank

    echo.seekg(0, ios::end);
    if (echo.tellg() == 0)
    {
        return;
    }

    // Print

    echo.seekg(0, ios::beg);

    cout << L"\n ┌─";
    for (size_t i = 0; i < echoBoxInsideWidth; ++i)
    {
        cout << L'─';
    }
    cout << L"─┐\n";

    string line;
    bool isFirstLine = true; // I forget why I made this...
    while (getline(echo, line))
    {
        if (line.empty()) // Blank line
        {
            Echo_BlankLine();
        }

        while (!line.empty())
        {
            string linePart = line.substr(0, echoBoxInsideWidth);
            if (line.size() > echoBoxInsideWidth)
            {
                if (size_t lastPeriod = linePart.rfind(". "); lastPeriod != string::npos)
                {
                    linePart.erase(lastPeriod + 2);
                    line.erase(0, lastPeriod + 2);
                }
                else if (size_t lastSpace = linePart.rfind(' '); lastSpace != string::npos)
                {
                    linePart.erase(lastSpace + 1);
                    line.erase(0, lastSpace + 1);
                }
                else
                {
                    line.erase(0, echoBoxInsideWidth);
                }
            }
            else
            {
                line.erase(0, echoBoxInsideWidth);
            }

            cout << L" │ ";
            cout << linePart;
            for (size_t i = linePart.size(); i < echoBoxInsideWidth; ++i)
            {
                cout << ' ';
            }
            cout << L" │\n";
        }
    }

    cout << L" └─";
    for (size_t i = 0; i < echoBoxInsideWidth; ++i)
    {
        cout << L'─';
    }
    cout << L"─┘\n";

    echo.str(string());
    echo.clear();
}
