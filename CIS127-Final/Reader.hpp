#pragma once
#include "SerializationUtils.hpp"
#include "ScopeHandler.hpp"

class ListReader;
class ObjectReader;

class Reader
{
public:
    Reader(istream& stream) :
        stream(stream) {}

private:
    // Extraction operator for all types except string. Getline for string.
    template<istreamable T>
    void _Extract(string value, T& result)
    {
        if ((istringstream(value) >> result).fail())
            throw new runtime_error("type mismatch for property; value was \"" + value + "\"");
    }

    // Returns the position of the character immediately following the separator.
    // May return the string's size, indicating the property name is the entire string (which is expected for scope heads).
    size_t _ExpectName(const string& line, const string& expectedName)
    {
        size_t nameStartPos = line.find_first_not_of(' ');
        if (nameStartPos == string::npos)
            throw new runtime_error("Line is empty or whitespace");

        size_t separatorPos = line.find(':');
        if (separatorPos == string::npos)
            throw new runtime_error("Line is missing property separator (expected `name: value`; missing ':')");

        string propertyName = line.substr(nameStartPos, separatorPos - nameStartPos);
        if (propertyName != expectedName)
            throw new runtime_error("Expected property \"" + expectedName + "\", got \"" + propertyName + "\"");

        return separatorPos + 1;
    }

    size_t _ReadIndent();

private: // Available to ListReader and ObjectReader
    friend class ScopeReader;

    size_t PeekIndent() const;
    void PopScope();

private: // Available to ListReader
    friend class ListReaderClient;

    unique_ptr<ListReader> BeginListElement();
    unique_ptr<ObjectReader> BeginObjectElement();

    template<istreamable T>
    void ReadElement(T& value)
    {
        string line;
        getline(stream, line);
        _Extract(line, value);
    }

private:// Available to ObjectReader
    friend class ObjectReaderClient;

    unique_ptr<ListReader> BeginListProperty(const string& expectedName);
    unique_ptr<ObjectReader> BeginObjectProperty(const string& expectedName);

    template<istreamable T>
    void ReadProperty(const string& expectedName, T& value)
    {
        assert_serialization_naming_standard(expectedName);

        try
        {
            size_t readIndent = _ReadIndent();
            size_t expectedIndent = scope.CurrentDepth();
            if (readIndent != expectedIndent)
                throw new runtime_error("expected indent depth of " + to_string(expectedIndent)  + "; got " + to_string(readIndent));

            string line;
            getline(stream, line);
            _Extract(line.substr(_ExpectName(line, expectedName)), value);
        }
        catch (runtime_error* err)
        {
            throw new runtime_error("On property " + expectedName + ":\n  " + err->what());
        }
    }

public:
    unique_ptr<ListReader> BeginList(const string& expectedName);
    unique_ptr<ObjectReader> BeginObject(const string& expectedName);

    template<istreamable T>
    void Read(const string& expectedName, T& value)
    {
        assert_serialization_naming_standard(expectedName);

        try
        {
            size_t readIndent = _ReadIndent();
            if (readIndent != 0)
                throw new runtime_error("expected no indentation; got " + to_string(readIndent));

            string line;
            getline(stream, line);
            _Extract(line.substr(_ExpectName(line, expectedName)), value);
        }
        catch (runtime_error* err)
        {
            throw new runtime_error("On global " + expectedName + ":\n  " + err->what());
        }
    }

private:
    ScopeHandler scope;
    istream& stream;
};

template<>
void Reader::_Extract<string>(string value, string& result);

template<>
void Reader::_Extract<bool>(string value, bool& result);
