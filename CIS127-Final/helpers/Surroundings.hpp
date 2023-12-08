#ifndef Surroundings_hpp
#define Surroundings_hpp

#include "utilities/pch/utilities.hpp"
#include "entities/Entity.hpp"

class Surroundings
{
public:
    ~Surroundings( );

    // Prints a list of the surroundings to the console
    void Print(ostream &stream) const;

    // Prompts the user with a list of the surroundings, including "self".
    EntityType Prompt(const string &prompt) const;

    bool Has(EntityType what) const;

    const Entity &Get(EntityType what) const;

    Entity &Get(EntityType what);

    bool IsEmpty( ) const;

    void Clear( );

    // Returns true if added successfully, otherwise false.
    bool TryAddNew(EntityType what);

    // Returns true if removed successfully, otherwise false.
    bool TryRemove(EntityType what);

    // Delegates removal to the next call of "RemoveQueued()".
    // This safely releases the entity without interrupting its function.
    void QueueForRemoval(EntityType what) const;

    // Releases all entities queued for removal. Call when entities are not in use.
    void ReleaseQueued( );

    void ReRoll( );

    EntityType Random( ) const;

    void Init( );
    void Save(ostream &ofs) const;
    void Load(istream &ifs);

private:
    map<EntityType, Entity *> things;
    mutable vector<EntityType> toRemove; // Does not actually modify surroundings, just modifies the list of things that will be modified later
};

extern Surroundings surroundings;

#endif /* Surroundings_hpp */
