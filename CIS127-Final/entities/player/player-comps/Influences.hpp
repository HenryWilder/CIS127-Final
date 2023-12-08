#pragma once
#include "IPlayerComponent.hpp"
#include "helpers/enums.hpp"

// Encompasses power, faith, and status; [-10..+10]
class Influences :
    public IPlayerComponent
{
public:
    // Returns the influence associated with the target collective.
    int GetInfluence(Collective targetCollective) const;

    // Returns the iterable collection of all influences.
    const map<Collective, int> &GetAllInfluence( ) const
    {
        return influence;
    }

    // Increases/decreases social status among the targeted collective (shortname) by the provided amount.
    // Clamps influence to range [-10..+10]; -50%, 200%, etc. success would be undefined behavior.
    void ModifyInfluence(Collective targetCollective, int change);

    // Erases all influence, effectively setting them to 0
    void ClearAllInfluence( )
    {
        influence.clear( );
    }

    // Roll a dice for a social check such that:
    // -10 influence gives a   0% chance of success
    //   0 influence gives a  50% chance of success
    // +10 influence gives a 100% chance of success
    bool CheckInfluence(Collective targetCollective) const;

    void Init( ) override;
    void Save(ostream &ofs) const override;
    void Load(istream &ifs) override;

private:
    map<Collective, int> influence;
};
