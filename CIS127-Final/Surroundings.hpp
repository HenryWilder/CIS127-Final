#ifndef Surroundings_hpp
#define Surroundings_hpp

#include "utilities.hpp"

class Interactable;

class Surroundings
{
public:
    ~Surroundings();
    
    // Prints a list of the surroundings to the console
    void Print() const;
    
    // Prompts the user with a list of the surroundings, followed by "self".
    string Prompt(const string& prompt) const;
    
    bool Has(const string& shortName) const;
    
    const Interactable& Get(const string& shortName) const;
    
    Interactable& Get(const string& shortName);
    
    bool IsEmpty() const;
    
    void Clear();
    
    // Returns true if added successfully, otherwise false.
    bool TryAddNew(const string& shortName);
    
    // Returns true if removed successfully, otherwise false.
    bool TryRemove(const string& shortName);
    
    void ReRoll();
    
    string RandomName() const;
    
    void Init(ostream& ofs) { ReRoll(); }
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
private:
    map<string, Interactable*> things;
};

extern Surroundings surroundings;

#endif /* Surroundings_hpp */
