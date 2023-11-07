#pragma once
#include <string>
#include <vector>
#include "utilities.hpp"
#include "randomness.hpp"
#include "Prompt.hpp"
using namespace std;

template<class _Enum>
struct StrEnum
{
    constexpr StrEnum(_Enum _enumeration, const char* _keyval) :
        enumeration(_enumeration), key(_keyval), value(_keyval) {}

    constexpr StrEnum(_Enum _enumeration, const char* _key, const char* _value) :
        enumeration(_enumeration), key(_key), value(_value) {}

    _Enum enumeration;
    const char* key;
    const char* value;

    bool operator==(      _Enum   test) const { return test == enumeration; }
    bool operator==(const string& test) const { return test == key;         }
};

template<class _Enum, size_t _Size>
struct StrEnumCollection
{
public:
    using Element_t = StrEnum<_Enum>;

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
    const char* KeyAt(_Enum enumeration) const
    {
        return At(enumeration).key;
    }
    const char* ValueAt(const string& key) const
    {
        return At(key).value;
    }
    const char* ValueAt(_Enum enumeration) const
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

    _Enum Prompt(const string& prompt) const
    {
        cout << prompt << '\n';
        for (const Element_t& element : data)
        {
            cout << "- " << element.key << '\n';
        }
        while (true)
        {
            auto it = Find(::Prompt());
            if (it != end(data)) return it->enumeration;
        }
    }

    template<integral_indexable_container_of<_Enum> _Container>
    _Enum Prompt(const string& prompt, const _Container& options) const
    {
        cout << prompt << '\n';
        List(options);
        while (true)
        {
            auto it = Find(::Prompt());
            if (it != end(data)) return it->enumeration;
        }
    }

    template<integral_indexable_container_of<_Enum> _Container>
    _Enum Prompt(const string& prompt, const _Container& options, const _Container& hiddenOptions) const
    {
        cout << prompt << '\n';
        List(options);
        while (true)
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
    const char* RandomValue() const
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

// First argument deduces _Enum.
// _Args deduces the number of parameters after the first.
// A StrEnumCollection cannot have 0 members, so there will always be at minimum 1 argument from which _Enum can be deduced.
template<class _Enum, same_as<StrEnum<_Enum>>... _Args>
StrEnumCollection(StrEnum<_Enum>, _Args...) -> StrEnumCollection<_Enum, (sizeof...(_Args) + 1)>;

// Defines the operators for the StrEnum type.
// This can be done by a macro because the operators are just aliases for members of the collection.
// It's simpler to do this by macro because it is a large amount of highly repetitive code that can't be done easily though a template.
#define STR_ENUM_OPERATORS(ENUM_CLASSNAME, COLLECTION) \
    inline ostream& operator<<(ostream& stream, ENUM_CLASSNAME enumeration) { return COLLECTION.WriteKey(stream, enumeration); } \
    inline istream& operator>>(istream& stream, ENUM_CLASSNAME &enumeration) { return COLLECTION.ReadKey(stream, enumeration); } \
    constexpr bool operator==(const string& str, ENUM_CLASSNAME enumeration) { return COLLECTION.Compare(str, enumeration); } \
    constexpr bool operator!=(const string& str, ENUM_CLASSNAME enumeration) { return !COLLECTION.Compare(str, enumeration); } \
    constexpr bool operator==(ENUM_CLASSNAME enumeration, const string& str) { return COLLECTION.Compare(str, enumeration); } \
    constexpr bool operator!=(ENUM_CLASSNAME enumeration, const string& str) { return !COLLECTION.Compare(str, enumeration); }

// Topic

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

STR_ENUM_OPERATORS(Topic, topics)

// Item

enum class Item
{
    Bread,  // Heals
    Sword,  // Damages; quantity represents durability
    Potion, // Effects are random
    Gold,   // 100% chance of getting items from NPCs willing to trade (higher chance than bread/talking)
};

constexpr StrEnumCollection items
{
    StrEnum{ Item::Bread,  "bread"  },
    StrEnum{ Item::Sword,  "sword"  },
    StrEnum{ Item::Potion, "potion" },
    StrEnum{ Item::Gold,   "gold"   },
};

STR_ENUM_OPERATORS(Item, items)

// Potion

// Sorted from most to least positive effects.
// First half are considered lucky when targeting a friend,
// second half are considered lucky when targeting an enemy
enum class Potion
{
    Predict, // Gives luck, providing an upper hand in anything the player is doing
    Heal,    // Heals the target
    Water,   // Douces the target in water - useless because fire is not DOT and there are no fire elementals
    Wish,    // Gives the target a wish; wish is random unless the target is the player
    Ducks,   // Summons ducks - useless
    Force,   // Pushes the target away - rerolls surroundings if player, removes target from surroundings otherwise
    Salt,    // Random chance to kill a monster
    Ants,    // Summons ants - useless
    Demon,   // Summons a random demon to attack the target
    Fire,    // Deals some damage
    Explode, // Deals high damage; can 1-hit most things including doors
    Tree,    // Turns the target into a tree, soft-locking the game if the target was the player.
    
    // Problem: Softlocking the game is a major enough risk that none of the effects are worth using potions on yourself.
    // Perhaps there could be a chance that NPCs can un-tree the player after one turn? Throwing away a turn is less destructive than a full-on soft-lock.
};

constexpr StrEnumCollection potions
{
    StrEnum{ Potion::Predict, "predict" },
    StrEnum{ Potion::Heal,    "heal"    },
    StrEnum{ Potion::Water,   "water"   },
    StrEnum{ Potion::Wish,    "wish"    },
    StrEnum{ Potion::Ducks,   "ducks"   },
    StrEnum{ Potion::Force,   "force"   },
    StrEnum{ Potion::Salt,    "salt"    },
    StrEnum{ Potion::Ants,    "ants"    },
    StrEnum{ Potion::Demon,   "demon"   },
    StrEnum{ Potion::Fire,    "fire"    },
    StrEnum{ Potion::Explode, "explode" },
    StrEnum{ Potion::Tree,    "tree"    },
};

STR_ENUM_OPERATORS(Potion, potions)

// Action

enum class Action
{
    Move,
    Talk,
    Grab,
    Use,
    Quit,
    Restart,
};

constexpr StrEnumCollection actions
{
    StrEnum{ Action::Move,    "move"    },
    StrEnum{ Action::Talk,    "talk"    },
    StrEnum{ Action::Grab,    "grab"    },
    StrEnum{ Action::Use,     "use"     },
    StrEnum{ Action::Quit,    "quit"    },
    StrEnum{ Action::Restart, "restart" },
};

STR_ENUM_OPERATORS(Action, actions)

// Direction

enum class Direction
{
    Left,
    Right,
    Forward,
};

constexpr StrEnumCollection directions
{
    StrEnum{ Direction::Left,    "left"    },
    StrEnum{ Direction::Right,   "right"   },
    StrEnum{ Direction::Forward, "forward" },
};

STR_ENUM_OPERATORS(Direction, directions)

// Collective

// Each NPC belongs to a random collective, giving the player a social stat among them
enum class Collective
{
    WesternExpanse,
    TwistingVacancy,
    InfinitePathways,
    NorthernSeed,
    EasternSwampfire,
    CriticalMalstrom,
    ElderKings,
    DeadKingCitadel,
    BabypunchingPuppykickers,
    Monsters,
};

constexpr StrEnumCollection collectives
{
    StrEnum{ Collective::WesternExpanse,           "western_expanse",           "citizens of the Western Expanse"                                },
    StrEnum{ Collective::TwistingVacancy,          "twisting_vacancy",          "Valley of the Twisting Vacancy"                                 },
    StrEnum{ Collective::InfinitePathways,         "infinite_pathways",         "Children of the Pond of Infinite Pathways"                      },
    StrEnum{ Collective::NorthernSeed,             "northern_seed",             "Followers of the Northern Seed"                                 },
    StrEnum{ Collective::EasternSwampfire,         "eastern_swampfire",         "Earthen Swampfire district of the Northeastern Bishop's Domain" },
    StrEnum{ Collective::CriticalMalstrom,         "critical_malstrom",         "Beholders of the Critical Malstrom"                             },
    StrEnum{ Collective::ElderKings,               "elder_kings",               "Nursing Home of the Elder Kings"                                },
    StrEnum{ Collective::DeadKingCitadel,          "dead_king_citadel",         "Holdout Clan of the Dead King's Fallen Citadel"                 },
    StrEnum{ Collective::BabypunchingPuppykickers, "babypunching_puppykickers", "League of Babypunching Puppykickers."                           },
    StrEnum{ Collective::Monsters,                 "monsters",                  "monsters"                                                       },
};

STR_ENUM_OPERATORS(Collective, collectives)