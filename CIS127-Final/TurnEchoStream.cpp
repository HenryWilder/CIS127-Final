#include "TurnEchoStream.hpp"

stringstream echo;

void FlushEcho()
{
    // Don't print blank
    if (echo.tellg() == 0)
    {
        return;
    }

    echo.seekg(0, ios::beg);

    cout << "\n\xDA";
    for (size_t i = 0; i < echoBoxInsideWidth + 2; ++i)
    {
        cout << '\xC4';
    }
    cout << "\xBF\n";

    string line;
    bool isFirstLine = true;
    while (getline(echo, line))
    {
        if (line.empty()) // Blank line
        {
            cout << "\xB3 ";
            for (size_t i = 0; i < echoBoxInsideWidth; ++i)
            {
                cout << ' ';
            }
            cout << " \xB3\n";
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

            cout << "\xB3 " << linePart;
            for (size_t i = linePart.size(); i < echoBoxInsideWidth; ++i)
            {
                cout << ' ';
            }
            cout << " \xB3\n";
        }
    }

    cout << "\xC0";
    for (size_t i = 0; i < echoBoxInsideWidth + 2; ++i)
    {
        cout << '\xC4';
    }
    cout << "\xD9\n";

    echo.str(string());
    echo.clear();
}
