#include "utilities.hpp"
#include "SerializationUtils.hpp"

ostream& Indent(ostream& stream, size_t depth)
{
    return stream << setfill(' ') << setw((streamsize)(indentSize * depth)) << "";
}
