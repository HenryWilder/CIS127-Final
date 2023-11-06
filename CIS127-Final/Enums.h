#pragma once
#include <string>
#include <tuple>
using namespace std;

template<class _Ty>
concept enum_type = is_enum_v<_Ty>;

template<enum_type _Enum>
constexpr size_t EnumSize = 0ull;

template<class _Enum>
struct StringableEnum
{
    constexpr StringableEnum(_Enum identifier, const char* text) :
        identifier(identifier), text(text) {}

    _Enum identifier;
    const char* text;
};

template<class _Enum>
class StringableEnumCollection
{
public:
    constexpr StringableEnumCollection(StringableEnum<_Enum> const (&_mapping)[EnumSize<_Enum>]) :
        mapping(_mapping) {}

    _Enum EnumFromString(const string& matchAgainst) const
    {
        for (const auto& [identifier, text] : mapping)
        {
            if (text == matchAgainst)
            {
                return identifier;
            }
        }
        throw new exception();
    }

    string EnumToString(_Enum matchAgainst) const
    {
        for (const auto& [identifier, text] : mapping)
        {
            if (identifier == matchAgainst)
            {
                return text;
            }
        }
        throw new exception();
    }

private:
    static constexpr _Size = _Enum::_Count;
    StringableEnum<_Enum> mapping[_Size];
};

template<class _Enum>
constexpr StringableEnumCollection<_Enum> convert = {};

enum class Topic
{
    WineFish,
    SkeleStock,
    WoodpeckerSiege,
    NecroFarm,
    Bloodmoon,
    MoleMountain,
    MindflayerRecipe,
    GodFistfight,
    Theseus,
    BlacksmithTeleken,
    NoGarlic,
    PenguinBattle,
    PetMentalHealth,
    Woodchuck,
};

template<>
constexpr size_t EnumSize<Topic> = (int)Topic::Woodchuck + 1;

template<>
constexpr StringableEnumCollection<Topic> convert({ StringableEnum
    { Topic::WineFish,          "the effects of water-wine alchemy on the local fish population"                                                          },
    { Topic::SkeleStock,        "the volatile stock price of enchanted skeleton armor"                                                                    },
    { Topic::WoodpeckerSiege,   "how many woodpeckers it would take to breach the castle wall"                                                            },
    { Topic::NecroFarm,         "the ethicacy of using necromancy in farming-related fields"                                                              },
    { Topic::Bloodmoon,         "when the next blood moon may occur"                                                                                      },
    { Topic::MoleMountain,      "whether enough moles could in fact make a mountain out of their hill"                                                    },
    { Topic::MindflayerRecipe,  "what recipes to use for preparing mind flayer"                                                                           },
    { Topic::GodFistfight,      "which gods could win in a fistfight against each other"                                                                  },
    { Topic::Theseus,           "why Theseus has been getting so fussy about their ship lately"                                                           },
    { Topic::BlacksmithTeleken, "what form of telekenesis would be the most effective for a blacksmith to use"                                            },
    { Topic::NoGarlic,          "how the elder wizards should handle the recent garlic & holy water defecits"                                             },
    { Topic::PenguinBattle,     "whether the Old Realm needs more supplies or troops to survive their war of attrition against the Penguin Guild"         },
    { Topic::PetMentalHealth,   "the effects of mind-altering spells on the mental health of familiars"                                                   },
    { Topic::Woodchuck,         "the quantity of wood throwable by a woodchuck in a hypothetical scenario that such a feat was possible for the creature" },
});

enum class Item
{
    Bread,
    Sword,
    Potion,
    Gold,
};

template<>
constexpr size_t EnumSize<Item> = (int)Item::Gold + 1;

extern const StringableEnumCollection<Item> items;
