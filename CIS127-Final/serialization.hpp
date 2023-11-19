#ifndef serialization_hpp
#define serialization_hpp

// Asks the user whether to load or create a game, then loads or initializes the game variables
void LoadWithPrompt();
void Save();

#if _DEBUG
void SerializationUnitTest();
#endif

#endif /* serialization_hpp */
