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

#define _Assert(cond, msgExpected, msgActual)\
    do {\
        cout << "assertion " << (!!(cond) ? COLOR_GREEN "passed" : COLOR_RED "failed") << COLOR_RESET " at " COLOR_YELLOW __FILE__ ":" << __LINE__ << ":5" << COLOR_RESET "\n";\
        if (!(cond))\
            cout << COLOR_BLUE "  expected   | " COLOR_GRAY msgExpected COLOR_RESET "\n"\
                    COLOR_BLUE "  actual     | " COLOR_GRAY  msgActual  COLOR_RESET "\n";\
        else\
            cout << COLOR_BLUE "  expression | " COLOR_GRAY msgExpected COLOR_RESET "\n";\
    } while (false)

#define AssertEquals(expected, actual)     _Assert( ((actual) == (expected)), #expected,        #actual      )
#define AssertNotEquals(expected, actual)  _Assert( ((actual) != (expected)), "not " #expected, #actual      )
#define AssertTrue(expr)                   _Assert( !!(expr),                 "true",           #expr        )
#define AssertFalse(expr)                  _Assert( !(expr),                  "false",          #expr        )
#define AssertNull(ptr)                    _Assert( (ptr == nullptr),         "null",           #ptr         )
#define AssertNotNull(ptr)                 _Assert( (ptr != nullptr),         "not null",       #ptr         )
#define AssertGreaterThan(a, b)            _Assert( ((a) >  (b)),             #a " > " #b,      #a " <= " #b )
#define AssertLessThan(a, b)               _Assert( ((a) <  (b)),             #a " < " #b,      #a " >= " #b )
#define AssertGreaterThanOrEqual(a, b)     _Assert( ((a) >= (b)),             #a " >= " #b,     #a " < " #b  )
#define AssertLessThanOrEqual(a, b)        _Assert( ((a) <= (b)),             #a " <= " #b,     #a " > " #b  )

void UnitTests::DoTests()
{
    // Test framework
    cout << "\nFramework tests\n";
    DoUnitTestFrameworkTests();

    // Test Utilities
    cout << "\nUtilities tests\n";
    DoUtilitiesTests();

    // Test Prompt
    cout << "\nPrompt tests\n";
    DoPromptTests();

    // Test Randomness
    cout << "\nRandomness tests\n";
    DoRandomnessTests();

    // Test Echo
    cout << "\nEcho tests\n";
    DoEchoTests();

    // Test Enums
    cout << "\nEnums tests\n";
    DoEnumsTests();

    // Test Surroundings
    cout << "\nSurroundings tests\n";
    DoSurroundingsTests();

    // Test TurnHandler
    cout << "\nTurnHandler tests\n";
    DoTurnHandlerTests();

    // Test Serialization
    cout << "\nSerialization tests\n";
    DoSerializationTests();

    // Test NPCs
    cout << "\nBaker tests\n";
    DoBakerTests();
    cout << "\nBlacksmith tests\n";
    DoBlacksmithTests();
    cout << "\nDoor tests\n";
    DoDoorTests();
    cout << "\nMonster tests\n";
    DoMonsterTests();
    cout << "\nWizard tests\n";
    DoWizardTests();

    // Test Player
    cout << "\nPlayer tests\n";
    DoPlayerTests();
}

void UnitTests::DoUnitTestFrameworkTests()
{
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

}

// Test Prompt
void UnitTests::DoPromptTests()
{

}

// Test Randomness
void UnitTests::DoRandomnessTests()
{

}

// Test Echo
void UnitTests::DoEchoTests()
{
    cout << "\nEcho Color tests\n";
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

}

// Test Surroundings
void UnitTests::DoSurroundingsTests()
{

}

// Test TurnHandler
void UnitTests::DoTurnHandlerTests()
{

}

// Test Serialization
void UnitTests::DoSerializationTests()
{

}

// Test NPCs
void UnitTests::DoBakerTests()
{

}
void UnitTests::DoBlacksmithTests()
{

}
void UnitTests::DoDoorTests()
{

}
void UnitTests::DoMonsterTests()
{

}
void UnitTests::DoWizardTests()
{

}

// Test Player
void UnitTests::DoPlayerTests()
{

}
