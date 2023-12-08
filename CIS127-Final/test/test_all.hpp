#pragma once

class UnitTests
{
public:
    // Run all tests
    static void DoTests();

    // Test the unit testing framework
    static void DoUnitTestFrameworkTests();

    // Test Utilities
    static void DoUtilitiesTests();

    // Test Prompt
    static void DoPromptTests();

    // Test Randomness
    static void DoRandomnessTests();

    // Test Echo
    static void DoEchoTests();

    // Test Enums
    static void DoEnumsTests();

    // Test Surroundings
    static void DoSurroundingsTests();

    // Test TurnHandler
    static void DoTurnHandlerTests();

    // Test Serialization
    static void DoSerializationTests();

    // Test NPCs
    static void DoBakerTests();
    static void DoBlacksmithTests();
    static void DoDoorTests();
    static void DoMonsterTests();
    static void DoWizardTests();

    // Test Player
    static void DoPlayerTests();
};
