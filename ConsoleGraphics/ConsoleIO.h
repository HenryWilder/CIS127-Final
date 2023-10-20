#pragma once
#include <HenryLib.h>
#include <vector>
#include <string>
#include <optional>
#include <iostream>
using std::cin;
using std::cout;
using std::getline;
using std::vector;
using std::pair;
using std::string;
using std::optional;

namespace cio
{
    template<typename _Ty>
    concept promptable = numeric<_Ty> || same_as<_Ty, string>;

    template<promptable _Ty>
    _Ty Prompt(const string& question)
    {
        cout << question;
        _Ty input;
        cin >> input;
        return input;
    }

    template<>
    string Prompt<string>(const string& question)
    {
        cout << question;
        string input;
        getline(cin, input);
        return input;
    }

    template<>
    bool Prompt<bool>(const string& question)
    {
        cout << question;
        while (true)
        {
            cout << " (y/n)";
            string input;
            cin >> input;
            input = input;

            if (input == "y" || input == "Y" || input == "yes" || input == "Yes") return true;
            if (input == "n" || input == "N" || input == "no"  || input == "No" ) return false;
        }
    }

    struct PromptOption
    {
        string code;
        string display;
        optional<string> description;
    };

    // Returns the index of the selected option
    size_t Prompt(const vector<PromptOption>& options);

    template<typename _ResultType>
    _ResultType Prompt(const vector<PromptOption>& inputs, const vector<_ResultType>& outputs)
    {
        return outputs.at(Prompt(inputs));
    }

    template<typename _ResultType>
    _ResultType Prompt(const vector<pair<PromptOption, _ResultType>>& options)
    {
        vector<PromptOption> inputs; 
        vector<_ResultType> outputs;

        inputs .reserve(options.size());
        outputs.reserve(options.size());

        for (const auto& [i, o] : options)
        {
             inputs.push_back(i);
            outputs.push_back(o);
        }
        return Prompt(inputs, outputs);
    }
}
