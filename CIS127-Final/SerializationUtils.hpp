#pragma once
#include "utilities.hpp"

class FileMalformedException :
    public exception
{
public:
    FileMalformedException() :
        msg("file malformed") {}

    FileMalformedException(const string& msg) :
        msg("file malformed: " + msg) {}

    const char* what() const noexcept override
    {
        return msg.c_str();
    }

private:
    string msg;
};

constexpr size_t indentSize = 2;

// Writers do not need to end with a newline. The SaveUtils::Save functions will do that automatically.
template<class _Ty> using Writer = void(*)(ostream& ofs, _Ty value);
template<class _Ty> using Reader =  _Ty(*)(istream& ifs);

template<class _Ty>
void DefaultWriter(ostream& ofs, _Ty value)
{
    ofs << value;
}

template<class _Ty>
_Ty DefaultReader(istream& ifs)
{
    _Ty value{};
    ifs >> value;
    return value;
}

class SaveUtils
{
private:
    static ostream& WriteName(ostream& ofs, const string& name)
    {
        return ofs << name << ": ";
    }

    static ostream& Indent(ostream& ofs, size_t depth)
    {
        return ofs << string(depth * indentSize, ' ');
    }

public:
    static void Save(ostream& ofs, const string& name, const bool value)
    {
        WriteName(ofs, name) << (value ? "true" : "false") << '\n';
    }

    static void Save(ostream& ofs, const string& name, const int value)
    {
        WriteName(ofs, name) << value << '\n';
    }

    static void Save(ostream& ofs, const string& name, const float value)
    {
        WriteName(ofs, name) << value << '\n';
    }

    static void Save(ostream& ofs, const string& name, const string& value)
    {
        WriteName(ofs, name) << value << '\n';
    }

    template<class _Ty, Writer _Writer = DefaultWriter<_Ty>>
    static void Save(ostream& ofs, const string& name, const vector<_Ty>& value)
    {
        WriteName(ofs, name) << value.size() << '\n';
        for (const _Ty& element : value)
        {
            Indent(ofs, 1);
            _Writer(element);
            ofs << '\n';
        }
    }

    template<class _Ty, Writer _Writer = DefaultWriter<_Ty>>
    static void Save(ostream& ofs, const string& name, const map<string, _Ty>& value)
    {
        WriteName(ofs, name) << value.size() << '\n';
        for (const auto& [key, val] : value)
        {
            Indent(ofs, 1) << key << ": ";
            _Writer(val);
            ofs << '\n';
        }
    }
};

class LoadUtils
{
private:
    static istream& ReadName(istream& ifs, string& readName)
    {
        getline(ifs, readName, ':').ignore(1);
        return ifs;
    }
    static istream& ExpectName(istream& ifs, const string& expectName)
    {
        string readName;
        ReadName(ifs, readName);
        if (readName != expectName)
        {
            throw new FileMalformedException(format("Expected \"{}\", got \"{}\"", expectName, readName));
        }
        return ifs;
    }

    static istream& ExpectIndentation(istream& ifs, size_t depth)
    {
        for (size_t i = 0; i < depth * indentSize; ++i)
        {
            char ch = ifs.get();
            if (ch != ' ')
            {
                throw new FileMalformedException(format(
                    "Expected indentation of {} spaces, read {} before encountering '{}'", depth * indentSize, i, ch));
            }
        }
        return ifs;
    }

public:
    static void Load(istream& ifs, const string& name, bool& value)
    {
        string written;
        ExpectName(ifs, name) >> written;
        if      (written == "true" ) value = true;
        else if (written == "false") value = false;
        else throw new FileMalformedException(format("Expected 'true' or 'false', got \"{}\"", written));
    }

    static void Load(istream& ifs, const string& name, int& value)
    {
        ExpectName(ifs, name) >> value;
    }

    static void Load(istream& ifs, const string& name, float& value)
    {
        ExpectName(ifs, name) >> value;
    }

    static void Load(istream& ifs, const string& name, string& value)
    {
        getline(ExpectName(ifs, name), value);
    }

    template<class _Ty, Reader _Reader = DefaultReader<_Ty>>
    static void Load(istream& ifs, const string& name, vector<_Ty>& value)
    {
        assert(value.empty()); // always clear vectors before loading to them
        size_t numElements;
        ExpectName(ifs, name) >> numElements;
        value.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            _Ty element;
            ExpectIndentation(ifs, 1);
            element = _Reader(ifs);
            value.push_back(element);
        }
    }

    template<class _Ty, Reader _Reader = DefaultReader<_Ty>>
    static void Load(istream& ifs, const string& name, map<string, _Ty>& value)
    {
        assert(value.empty()); // always clear maps before loading to them
        size_t numElements;
        ExpectName(ifs, name) >> numElements;
        for (size_t i = 0; i < numElements; ++i)
        {
            string key;
            _Ty val;
            ReadName(ExpectIndentation(ifs, 1), key);
            val = _Reader(ifs);
            value.emplace(key, val);
        }
    }
};
