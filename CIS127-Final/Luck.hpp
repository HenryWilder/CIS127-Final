#pragma once
#include "IPlayerComponent.hpp"

enum class LuckType
{
    Good = +1,
    Neutral = 0,
    Bad = -1,
};

class Luck :
    public IPlayerComponent
{
private:
    static constexpr LuckType CategorizeLuck(int amount)
    {
        if (amount > 0) return LuckType::Good;
        if (amount < 0) return LuckType::Bad;
        return LuckType::Neutral;
    }

public:
    // If and only if the provided check would have failed naturally, one unit of good luck is consumed
    // to override the failure and replace it with a guaranteed success.
    //
    // If and only if the provided check would have succeeded naturally, one unit of bad luck is consumed
    // to override the success and replace it with a guaranteed failure.
    //
    // If the player has no luck--good nor bad--the success will not influenced and no luck be consumed.
    bool ApplyLuck(bool success);
    
    // Returns the current type of luck (good/bad/neutral).
    // Consumes 1 unit of good luck if good.
    // Consumes 1 unit of bad luck if bad.
    // Consumes nothing if luck is neutral.
    LuckType CheckLuck();

    // "Testing your luck"
    // Returns the current type of luck (good/bad/neutral).
    // Consumes 1 unit of good luck no matter what.
    // If luck is already bad, it gets worse.
    LuckType TestLuck();
    
    void GiveLuck(_In_range_(!=, LuckType::Neutral) LuckType kind, _In_range_(>, 0) int amount = 1);
    
    // Removes all luck, both good and bad
    void ClearAllLuck( )
    {
        luck = 0;
    }

    void Init() override;
    void Save(ostream& ofs) const override;
    void Load(istream& ifs) override;
    
private:
    int luck = 0;
};
