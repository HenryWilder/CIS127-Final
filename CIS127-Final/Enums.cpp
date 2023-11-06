#include "Enums.h"
#include "utilities.hpp"
#include <map>

struct Empty {};

template<class _Ty>
using type_or_empty_t = conditional_t<is_same_v<_Ty, void>, Empty, _Ty>;

template<class _Ty>
using needs_arg_t = enable_if_t<negation_v<is_same<_Ty, void>>, bool>;

template<class _Ty>
using forbids_arg_t = enable_if_t<is_same_v<_Ty, void>, bool>;

template<class _Enum, class _Valty>
struct StrEnum
{
    using forbids_valty = forbids_arg_t<_Valty>;
    using needs_valty   = needs_arg_t<_Valty>;

    template<forbids_valty = true>
    constexpr StrEnum(_Enum _enumeration, const char* _key) :
        enumeration(_enumeration), key(_key), value(default) {}

    template<needs_valty = true>
    constexpr StrEnum(_Enum _enumeration, const char* _key, _Valty _value) :
        enumeration(_enumeration), key(_key), value(_value) {}

    _Enum enumeration;
    const char* key;
    type_or_empty_t<_Valty> value;

    bool operator==(      _Enum   test) const { return test == enumeration; }
    bool operator==(const string& test) const { return test == key;         }
};

template<class _Enum>
StrEnum(_Enum, const char*) -> StrEnum<_Enum, void>;

template<class _Enum, class _Valty>
StrEnum(_Enum, const char*, _Valty) -> StrEnum<_Enum, _Valty>;

template<class _Enum, class _Valty, size_t _Size>
struct StrEnumCollection
{
public:
    using Element_t     = StrEnum<_Enum, _Valty>;
    using forbids_valty = typename Element_t::forbids_valty;
    using needs_valty   = typename Element_t::needs_valty;

    template<same_as<Element_t>... _Args>
    constexpr StrEnumCollection(_Args... args) :
        data{ args... } {}

    const Element_t& At(const string& key) const
    {
        auto it = Find(key);
        if (it != end(data)) return *it;
        throw new exception();
    }
    const Element_t& At(_Enum enumeration) const
    {
        auto it = Find(enumeration);
        if (it != end(data)) return *it;
        throw new exception();
    }

    const Element_t& operator[](const string& key) const
    {
        return At(key);
    }
    const Element_t& operator[](_Enum enumeration) const
    {
        return At(enumeration);
    }

    _Enum EnumAt(const string& key) const
    {
        return At(key).enumeration;
    }
    string KeyAt(_Enum enumeration) const
    {
        return At(enumeration).key;
    }

    template<needs_valty = true>
    _Valty ValueAt(const string& key) const
    {
        return At(key).value;
    }

    template<needs_valty = true>
    _Valty ValueAt(_Enum enumeration) const
    {
        return At(enumeration).value;
    }

    ostream& WriteKey(ostream& stream, _Enum enumeration) const
    {
        return stream << At(enumeration).key;
    }
    istream& ReadKey(istream& stream, _Enum& enumeration) const
    {
        string str;
        stream >> str;
        enumeration = At(str).enumeration;
        return stream;
    }

    constexpr bool Compare(const string& str, _Enum enumeration) const
    {
        const Element_t* element = Find(enumeration);
        return (element == end(data)) && (element->key == str);
    }

    _Enum Prompt(const string& prompt, const vector<_Enum>& options) const
    {
        cout << prompt << '\n';
        List(options);
        loop
        {
            auto it = Find(::Prompt());
            if (it != end(data)) return it->enumeration;
        }
    }

    Element_t Random() const
    {
        return ChooseRandom(begin(data), end(data));
    }
    _Enum RandomEnum() const
    {
        return Random().enumeration;
    }
    const char* RandomKey() const
    {
        return Random().key;
    }
    template<needs_valty = true>
    _Valty RandomValue() const
    {
        return Random().value;
    }

private: // Helpers
    constexpr const Element_t* Find(const string& key) const
    {
        return find(begin(data), end(data), key);
    }
    constexpr const Element_t* Find(_Enum enumeration) const
    {
        return find(begin(data), end(data), enumeration);
    }

private: // Members
    Element_t data[_Size];
};

// First argument deduces the types of _Enum and _Valty.
// _Args deduces the number of args after the first.
// A StrEnumCollection cannot have 0 members, so there should always be at minimum 1 argument from which _Enum & _Valty can be deduced.
template<class _Enum, class _Valty, class... _Args>
StrEnumCollection(StrEnum<_Enum, _Valty>, _Args...) -> StrEnumCollection<_Enum, _Valty, (sizeof...(_Args) + 1)>;

// Topic

constexpr StrEnumCollection topics
{
    StrEnum{ Topic::WineFish,          "wine_fish",          "the effects of water-wine alchemy on the local fish population"                                                          },
    StrEnum{ Topic::SkeleStock,        "skele_stock",        "the volatile stock price of enchanted skeleton armor"                                                                    },
    StrEnum{ Topic::WoodpeckerSiege,   "woodpecker_siege",   "how many woodpeckers it would take to breach the castle wall"                                                            },
    StrEnum{ Topic::NecroFarm,         "necro_farm",         "the ethicacy of using necromancy in farming-related fields"                                                              },
    StrEnum{ Topic::Bloodmoon,         "bloodmoon",          "when the next blood moon may occur"                                                                                      },
    StrEnum{ Topic::MoleMountain,      "mole_mountain",      "whether enough moles could in fact make a mountain out of their hill"                                                    },
    StrEnum{ Topic::MindflayerRecipe,  "mindflayer_recipe",  "what recipes to use for preparing mind flayer"                                                                           },
    StrEnum{ Topic::GodFistfight,      "god_fistfight",      "which gods could win in a fistfight against each other"                                                                  },
    StrEnum{ Topic::Theseus,           "theseus",            "why Theseus has been getting so fussy about their ship lately"                                                           },
    StrEnum{ Topic::BlacksmithTeleken, "blacksmith_teleken", "what form of telekenesis would be the most effective for a blacksmith to use"                                            },
    StrEnum{ Topic::NoGarlic,          "no_garlic",          "how the elder wizards should handle the recent garlic & holy water defecits"                                             },
    StrEnum{ Topic::PenguinBattle,     "penguin_battle",     "whether the Old Realm needs more supplies or troops to survive their war of attrition against the Penguin Guild"         },
    StrEnum{ Topic::PetMentalHealth,   "pet_mental_health",  "the effects of mind-altering spells on the mental health of familiars"                                                   },
    StrEnum{ Topic::Woodchuck,         "woodchuck",          "the quantity of wood throwable by a woodchuck in a hypothetical scenario that such a feat was possible for the creature" },
};

ostream& operator<<(ostream& stream, Topic identifier)
{
    return topics.WriteKey(stream, identifier);
}
istream& operator>>(istream& stream, Topic& identifier)
{
    return topics.ReadKey(stream, identifier);
}
bool operator==(const string& str, Topic identifier)
{
    return topics.Compare(str, identifier);
}
bool operator==(Topic identifier, const string& str)
{
    return topics.Compare(str, identifier);
}

// Item

constexpr StrEnumCollection items
{
    StrEnum{ Item::Bread,  "bread",  "bread"  },
    StrEnum{ Item::Sword,  "sword",  "sword"  },
    StrEnum{ Item::Potion, "potion", "potion" },
    StrEnum{ Item::Gold,   "gold",   "gold"   },
};

ostream& operator<<(ostream& stream, Item identifier)
{
    return items.WriteKey(stream, identifier);
}
istream& operator>>(istream& stream, Item& identifier)
{
    return items.ReadKey(stream, identifier);
}
bool operator==(const string& str, Item identifier)
{
    return items.Compare(str, identifier);
}
bool operator==(Item identifier, const string& str)
{
    return items.Compare(str, identifier);
}

Item Prompt(const string& prompt, const vector<Item>& options)
{
    return items.Prompt(prompt, options);
}

// Potion

constexpr StrEnumCollection potions
{
    StrEnum{ Potion::Predict, "predict", "predict" },
    StrEnum{ Potion::Heal,    "heal",    "heal"    },
    StrEnum{ Potion::Water,   "water",   "water"   },
    StrEnum{ Potion::Wish,    "wish",    "wish"    },
    StrEnum{ Potion::Ducks,   "ducks",   "ducks"   },
    StrEnum{ Potion::Force,   "force",   "force"   },
    StrEnum{ Potion::Salt,    "salt",    "salt"    },
    StrEnum{ Potion::Ants,    "ants",    "ants"    },
    StrEnum{ Potion::Demon,   "demon",   "demon"   },
    StrEnum{ Potion::Fire,    "fire",    "fire"    },
    StrEnum{ Potion::Explode, "explode", "explode" },
    StrEnum{ Potion::Tree,    "tree",    "tree"    },
};

ostream& operator<<(ostream& stream, Potion identifier)
{
    return potions.WriteKey(stream, identifier);
}
istream& operator>>(istream& stream, Potion& identifier)
{
    return potions.ReadKey(stream, identifier);
}
bool operator==(const string& str, Potion identifier)
{
    return potions.Compare(str, identifier);
}
bool operator==(Potion identifier, const string& str)
{
    return potions.Compare(str, identifier);
}
