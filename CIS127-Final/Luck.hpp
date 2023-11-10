#ifndef Luck_hpp
#define Luck_hpp

#include "utilities.hpp"

class Luck :
    private NotCopyable
{
public:
    enum LuckType
    {
        Good    = +1,
        Neutral =  0,
        Bad     = -1,
    };

private:
    static constexpr LuckType CategorizeLuck(int amount)
    {
        return amount > 0 ? Good : amount < 0 ? Bad : Neutral;
    }

public:
    // If and only if the provided check would have failed naturally, one unit of good luck is consumed
    // to override the failure and replace it with a guaranteed success.
    //
    // If and only if the provided check would have succeeded naturally, one unit of bad luck is consumed
    // to override the success and replace it with a guaranteed failure.
    //
    // If the player has no luck--good nor bad--the success will not influenced and no luck be consumed.
    bool Apply(bool success);
    
    // Returns the current type of luck (good/bad/neutral).
    // Consumes 1 unit of good luck if good.
    // Consumes 1 unit of bad luck if bad.
    // Consumes nothing if luck is neutral.
    LuckType Check();

    // "Testing your luck"
    // Returns the current type of luck (good/bad/neutral).
    // Consumes 1 unit of good luck no matter what.
    // If luck is already bad, it gets worse.
    LuckType Test();
    
    void Give(_In_range_(!=, Neutral) LuckType kind, _In_range_(>, 0) int amount = 1);
    
    // Removes all luck, both good and bad
    void Clear();

    void Init();
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
private:
    int luck = 0;
};

#endif /* Luck_hpp */
