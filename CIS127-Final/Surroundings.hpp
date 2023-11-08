#ifndef Surroundings_hpp
#define Surroundings_hpp

#include "utilities.hpp"
#include "Interactable.hpp"

class Surroundings
{
public:
    ~Surroundings();
    
    // Prints a list of the surroundings to the console
    void Print() const;
    
    // Prompts the user with a list of the surroundings, including "self".
    EntityType Prompt(const string& prompt) const;
    
    bool Has(EntityType what) const;
    
    const Entity& Get(EntityType what) const;
    
    Entity& Get(EntityType what);
    
    bool IsEmpty() const;
    
    void Clear();
    
    // Returns true if added successfully, otherwise false.
    bool TryAddNew(EntityType what);
    
    // Returns true if removed successfully, otherwise false.
    bool TryRemove(EntityType what);
    
    void ReRoll();
    
    EntityType Random() const;
    
    void Init() { ReRoll(); }
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
private:
    map<EntityType, Entity*> things;
};

extern Surroundings surroundings;

#endif /* Surroundings_hpp */
