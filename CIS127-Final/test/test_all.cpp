#include "utilities/pch/utilities.hpp"
#include "test_all.hpp"
#include "utilities/prompt.hpp"
#include "utilities/randomness.hpp"
#include "helpers/echo.hpp"
#include "helpers/enums.hpp"
#include "helpers/Surroundings.hpp"
#include "helpers/TurnHandler.hpp"
#include "serialization/serialization.hpp"
#include "entities/npcs/Baker.hpp"
#include "entities/npcs/Blacksmith.hpp"
#include "entities/npcs/Door.hpp"
#include "entities/npcs/Monster.hpp"
#include "entities/npcs/Wizard.hpp"
#include "entities/player/Player.hpp"

// constexpr int EXPRESSION_WIDTH = 25;

#define _Assert(cond, msgExpected, msgExpectedValue, msgActual, msgActualValue)\
    do {\
        cout << "assertion " << (!!(cond) ? COLOR_GREEN "passed" : COLOR_RED "failed") << COLOR_RESET " at " COLOR_YELLOW __FILE__ ":" << __LINE__ << ":5" << COLOR_RESET "\n";\
        if (!(cond))\
            cout << COLOR_BLUE "  expected   | " COLOR_GRAY msgExpected COLOR_MAGENTA << ' ' << msgExpectedValue << COLOR_RESET "\n"\
                    COLOR_BLUE "  actual     | " COLOR_GRAY  msgActual  COLOR_MAGENTA << ' ' <<  msgActualValue  << COLOR_RESET "\n";\
        else\
            cout << COLOR_BLUE "  expression | " COLOR_GRAY msgExpected COLOR_MAGENTA << ' ' << msgExpectedValue << COLOR_RESET "\n";\
    } while (false)

#define AssertEquals(expected, actual)         _Assert(((actual) == (expected)),                #expected, expected, #actual, actual)
#define AssertNotEquals(expected, actual)      _Assert(((actual) != (expected)),         "not " #expected, expected, #actual, actual)
#define AssertTrue(expr)                       _Assert(!!(expr),                         "true",           true,     #expr,   expr  )
#define AssertFalse(expr)                      _Assert(!(expr),                          "false",          false,    #expr,   expr  )
#define AssertNull(ptr)                        _Assert((ptr == nullptr),                 "null",           nullptr,  #ptr,    ptr   )
#define AssertNotNull(ptr)                     _Assert((ptr != nullptr),                 "not null",       "!0",     #ptr,    ptr   )
#define AssertGreaterThan(a, b)                _Assert(((a) > (b)),                      #a " > "  #b,     a,        #b,      b     )
#define AssertLessThan(a, b)                   _Assert(((a) < (b)),                      #a " < "  #b,     a,        #b,      b     )
#define AssertGreaterThanOrEqual(a, b)         _Assert(((a) >= (b)),                     #a " >= " #b,     a,        #b,      b     )
#define AssertLessThanOrEqual(a, b)            _Assert(((a) <= (b)),                     #a " <= " #b,     a,        #b,      b     )

/**
 * @brief This function is used to mark the beginning of a group of tests.
 *
 * @param name The name of the region to be tested. Should be formatted as a singular noun or noun phrase.
 */
void TestRegion(const char* name)
{
    cout << "\n" COLOR_CYAN << name << " tests" COLOR_RESET "\n";
}

void UnitTests::DoTests()
{
    using TestFunction = void (*)();
    TestFunction tests[] = {
        DoUnitTestFrameworkTests,
        DoUtilitiesTests,
        DoPromptTests,
        DoRandomnessTests,
        DoEchoTests,
        DoEnumsTests,
        DoSurroundingsTests,
        DoTurnHandlerTests,
        DoSerializationTests,
        DoBakerTests,
        DoBlacksmithTests,
        DoDoorTests,
        DoMonsterTests,
        DoWizardTests,
        DoPlayerTests
    };

    for (const TestFunction& test : tests)
    {
        test();
    }
}

void UnitTests::DoUnitTestFrameworkTests()
{
    TestRegion("Framework");

    int number = 7;
    int biggerNumber = number + 1;
    int smallerNumber = number - 1;

    AssertTrue(true);
    AssertTrue(false);

    AssertEquals(number, number);
    AssertNotEquals(number, biggerNumber);

    #pragma warning(push)
    #pragma warning(disable: 4127)
    AssertNull(nullptr);
    #pragma warning(pop)

    AssertNotNull(&number);

    AssertGreaterThan(biggerNumber, smallerNumber);

    AssertLessThan(smallerNumber, biggerNumber);

    AssertGreaterThanOrEqual(biggerNumber, smallerNumber);
    AssertGreaterThanOrEqual(number, number);

    AssertLessThanOrEqual(smallerNumber, biggerNumber);
    AssertLessThanOrEqual(number, number);
}

// Test Utilities
void UnitTests::DoUtilitiesTests()
{
    TestRegion("Utilities");

    TestRegion("IsExactlyOneBitSet");
    AssertTrue(IsExactlyOneBitSet(2));
    AssertTrue(IsExactlyOneBitSet(4));
    AssertTrue(IsExactlyOneBitSet(8));
    AssertFalse(IsExactlyOneBitSet(3));
    AssertFalse(IsExactlyOneBitSet(5));
    AssertFalse(IsExactlyOneBitSet(6));

    TestRegion("Trim");
    AssertEquals(Trim("    apple    "), "apple");
    AssertEquals(Trim("orange"), "orange");
    AssertEquals(Trim("banana  \n  "), "banana");
    AssertEquals(Trim("  \t  mango"), "mango");

    TestRegion("IsEmptyOrWhitespace");
    AssertTrue(IsEmptyOrWhitespace(""));
    AssertTrue(IsEmptyOrWhitespace("        "));
    AssertTrue(IsEmptyOrWhitespace("\n\t\n  "));
    AssertFalse(IsEmptyOrWhitespace("apple"));
    AssertFalse(IsEmptyOrWhitespace("        .   "));

    TestRegion("isvowel");
    AssertTrue(isvowel('a'));
    AssertTrue(isvowel('e'));
    AssertTrue(isvowel('i'));
    AssertFalse(isvowel('b'));
    AssertFalse(isvowel('c'));
    AssertFalse(isvowel('d'));
    AssertFalse(isvowel('1'));
    AssertFalse(isvowel('&'));
    AssertFalse(isvowel('\x1B'));
}

// Test Prompt
void UnitTests::DoPromptTests()
{
    // TODO
}

// Test Randomness
void UnitTests::DoRandomnessTests()
{
    // TODO
}

// Test Echo
void UnitTests::DoEchoTests()
{
    TestRegion("Echo Color");
    echo << COLOR_BLACK   "This text is black.\n";
    echo << COLOR_RED     "This text is red.\n";
    echo << COLOR_GREEN   "This text is green.\n";
    echo << COLOR_YELLOW  "This text is yellow.\n";
    echo << COLOR_BLUE    "This text is blue.\n";
    echo << COLOR_MAGENTA "This text is magenta.\n";
    echo << COLOR_CYAN    "This text is cyan.\n";
    echo << COLOR_WHITE   "This text is white.\n";
    echo << COLOR_GRAY    "This text is gray.\n";
    FlushEcho();
}

// Test Enums
void UnitTests::DoEnumsTests()
{
    // TODO
}

// Test Surroundings
void UnitTests::DoSurroundingsTests()
{
    // TODO
}

// Test TurnHandler
void UnitTests::DoTurnHandlerTests()
{
    // TODO
}

void LoadFromFile(ifstream& ifs); // Defined in `serialization.cpp`
void SaveToFile  (ofstream& ofs); // Defined in `serialization.cpp`

// Test Serialization
void UnitTests::DoSerializationTests()
{
    TestRegion("Serialization");

    const int bread  = 5867;
    const int gold   = 9436;
    const int potion = 3385;
    const int sword  = 6654;

    {
        ofstream testFile("save-test.txt");
        player.Init();
        player.AddItem(Item::Bread,  bread );
        player.AddItem(Item::Gold,   gold  );
        player.AddItem(Item::Potion, potion);
        player.AddItem(Item::Sword,  sword );
        surroundings.ReRoll();
        if (testFile.is_open())
        {
            SaveToFile(testFile);
            testFile.close();
        }
    }
    {
        ifstream testFile("save-test.txt");
        player.Init();
        surroundings.Clear();
        AssertTrue(testFile.is_open()); // Test that the file was created by SaveToFile().
        if (testFile.is_open())
        {
            LoadFromFile(testFile);
            AssertEquals(player.CountItem(Item::Bread),  bread ); // Test deserialization
            AssertEquals(player.CountItem(Item::Gold),   gold  ); // Test deserialization
            AssertEquals(player.CountItem(Item::Potion), potion); // Test deserialization
            AssertEquals(player.CountItem(Item::Sword),  sword ); // Test deserialization

            // Print file contents
            {
                string line;
                cout << "File Contents:" << endl;
                while (getline(testFile, line))
                {
                    cout << line << endl;
                }
            }

            testFile.close();
        }
    }
}

// Test NPCs
void UnitTests::DoBakerTests()
{
    // TODO
}
void UnitTests::DoBlacksmithTests()
{
    // TODO
}
void UnitTests::DoDoorTests()
{
    // TODO
}
void UnitTests::DoMonsterTests()
{
    // TODO
}
void UnitTests::DoWizardTests()
{
    // TODO
}

// Test Player
void UnitTests::DoPlayerTests()
{
    // TODO
}
