#ifndef Luck_hpp
#define Luck_hpp

#include "utilities.hpp"

class Luck :
    private NotCopyable
{
public:
    // If and only if the provided check would have failed naturally, one unit of good luck is consumed
    // to override the failure and replace it with a guaranteed success.
    //
    // If and only if the provided check would have succeeded naturally, one unit of bad luck is consumed
    // to override the success and replace it with a guaranteed failure.
    //
    // If the player has no luck--good nor bad--the success will not influenced and no luck be consumed.
    bool Apply(bool success);
    
    // Returns -1, 0, or +1.
    // If the player has good luck, returns +1 and consumes one unit of good luck.
    // If the player has  bad luck, returns -1 and consumes one unit of bad luck.
    // If the player has   no luck, returns  0 and consumes no luck.
    int Check();
    
    // Positive = good luck
    // Negative =  bad luck
    void Give(int amount);
    
    // Removes all luck, both good and bad
    void Clear();
    
    // Should only be used for loading
    void Set(int amount);
    
    friend void Save();
    friend void Load();
    
private:
    int luck = 0;
};

#endif /* Luck_hpp */
