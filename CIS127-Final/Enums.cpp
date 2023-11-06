#include "Enums.h"

const StringableEnumCollection topics

Topic TopicFromString(const string& value)
{
    return topics.EnumFromString(value);
}

string TopicToString(Topic key)
{
    return topics.EnumToString(key);
}
