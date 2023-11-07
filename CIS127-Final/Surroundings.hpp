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
    InteractableType Prompt(const string prompt) const;
    
    bool Has(InteractableType what) const;
    
    const Interactable& Get(InteractableType what) const;
    
    Interactable& Get(InteractableType what);
    
    bool IsEmpty() const;
    
    void Clear();
    
    // Returns true if added successfully, otherwise false.
    bool TryAddNew(InteractableType what);
    
    // Returns true if removed successfully, otherwise false.
    bool TryRemove(InteractableType what);
    
    void ReRoll();
    
    InteractableType Random() const;
    
    void Init() { ReRoll(); }
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
private:
    map<InteractableType, Interactable*> things;
};

extern Surroundings surroundings;

#endif /* Surroundings_hpp */
