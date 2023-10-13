#pragma once
#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::initializer_list;
using std::cin;
using std::cout;
using std::endl;

struct MenuFlavor
{
    void(*echoSelection)(const string& friendlyName) = +[](const string& friendlyName) {
        cout << "You have selected " << friendlyName << ".";
    };

    void(*unrecognizedOption)(const string& input) = +[](const string& input) {
        cout << "\"" << input << "\" is not a recognized option. Please select one of the options listed above.";
    };
};

template<typename _Token>
struct Menu
{
    struct Item
    {
        string userInput;
        string friendlyName;
        _Token token;
    };

    MenuFlavor flavor;
    vector<Item> options;

    _Token GetSelection() const
    {
        for (const Item item : options)
        {
            cout << item.userInput << ": " << item.friendlyName << '\n';
        }

        while (true)
        {
            cout << "> ";
            string input;
            getline(cin, input);
            for (const Item item : options)
            {
                if (input == item.userInput)
                {
                    flavor.echoSelection(item.friendlyName);
                    cout << endl;
                    return item.token;
                }
            }
            flavor.unrecognizedOption(input);
            cout << endl;
        }
    }
};
