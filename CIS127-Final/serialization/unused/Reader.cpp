#include "utilities/pch/utilities.hpp"
#if 0
#include "Reader.hpp"
#include "ListReader.hpp"

size_t Reader::_ReadIndent()
{
    size_t indent = 0;
    for (; stream.peek() == ' '; ++indent)
    {
        stream.ignore();
    }

    if (indent % indentSize != 0)
        throw new runtime_error("file malformed; indentation is not divisible by " + to_string(indentSize));

    return indent / 2;
}

size_t Reader::PeekIndent() const
{
    size_t indent = 0;
    {
        streampos initialPos = stream.tellg();
        char ch;
        stream >> ch;
        for (; ch == ' '; ++indent)
        {
            stream >> ch;
        }
        stream.seekg(initialPos);
    }

    if (indent % indentSize != 0)
        throw new runtime_error("file malformed; indentation is not divisible by " + to_string(indentSize));

    return indent / 2;
}

void Reader::PopScope( )
{
    scope.PopScope( );
}

unique_ptr<ListReader> Reader::BeginListElement()
{
    // TODO: expect indentation
    scope.PushScope(ScopeType::List);
    return make_unique<ListReader>(*this, scope.CurrentDepth( ));
}

unique_ptr<ObjectReader> Reader::BeginObjectElement()
{
    // TODO: expect indentation
    scope.PushScope(ScopeType::Object);
    return make_unique<ObjectReader>(*this, scope.CurrentDepth( ));
}

unique_ptr<ListReader> Reader::BeginListProperty(const string &expectedName)
{
    assert_serialization_naming_standard(expectedName);
    string line;
    getline(stream, line);
    _ExpectName(line, expectedName);
    scope.PushScope(ScopeType::List);
    return make_unique<ListReader>(*this, scope.CurrentDepth( ));
}

unique_ptr<ObjectReader> Reader::BeginObjectProperty(const string& expectedName)
{
    assert_serialization_naming_standard(expectedName);
    string line;
    getline(stream, line);
    _ExpectName(line, expectedName);
    scope.PushScope(ScopeType::Object);
    return make_unique<ObjectReader>(*this, scope.CurrentDepth( ));
}

runtime_error* GenerateExctractException(const string& value, const string& descr, const string& got, const string& remarks = "")
{
    return new runtime_error(format("String property value (\"{}\") is {} (expected `name: \"value\"`; got `name: {}`). {}", value, descr, got, remarks));
}

template<>
void Reader::_Extract<string>(string value, string& result)
{
    size_t firstNonWS, lastNonWS;

    firstNonWS = value.find_first_not_of(" \t");

    if (firstNonWS == string::npos)
        throw GenerateExctractException(value, "entirely missing/whitespace", "", "Is this property meant to be scoped?");

    lastNonWS = value.find_last_not_of(" \t");

    bool isMissingOpenQuote = value.at(firstNonWS) != '\"';
    bool isMissingCloseQuote = value.at(lastNonWS) != '\"';

    if (isMissingOpenQuote && isMissingCloseQuote)
        throw GenerateExctractException(value, "not contained within quote marks", "value");

    else if (isMissingOpenQuote)
        throw GenerateExctractException(value, "not preceeded with a opening quote mark", "value\"");

    else if (isMissingCloseQuote)
        throw GenerateExctractException(value, "not followed by a closing quote mark", "\"value", "Mult-line strings are not supported.");

    else if (firstNonWS == lastNonWS)
        throw GenerateExctractException(value, "malformed", "\"", "Mult-line strings are not supported.");

    result = value.substr(firstNonWS + 1, lastNonWS - firstNonWS - 1);
}

template<>
void Reader::_Extract<bool>(string value, bool& result)
{
    if (value == "true") result = true;
    else if (value == "false") result = false;
    else throw new runtime_error("type mismatch for property; value was \"" + value + "\"");
}

#endif
