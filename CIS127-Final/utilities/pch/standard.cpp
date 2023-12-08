#include "utilities/pch/utilities.hpp"
#include "standard.hpp"

#if _DEBUG
void PrettyError(const string& errorType, const string& file, const string& function, size_t line, const string& msg)
{
    auto HorizontalRule = +[] { cerr << setw(50) << '\n'; };
    size_t fileNameBegin = file.find_last_of('\\');
    if (fileNameBegin == string::npos) fileNameBegin = 0;

    string simplifiedFunction = function;
    simplifiedFunction = regex_replace(simplifiedFunction, regex("std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"), "std::string");

    cerr << setfill('-');
    HorizontalRule();
    cerr << "ERROR: "    << errorType << '\n';
    cerr << "File: "     << file.substr(fileNameBegin) << '\n';
    cerr << "Function: " << simplifiedFunction << '\n';
    cerr << "Line: "     << line << '\n';
    cerr << "Message:\n";
    {
        istringstream iss(msg);
        string msgLine;
        while (getline(iss, msgLine))
        {
            cerr << "| " << msgLine << '\n';
        }
    }
    HorizontalRule();
    cerr << setfill(' ');
}
#endif
