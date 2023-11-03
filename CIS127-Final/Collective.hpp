#ifndef Collective_hpp
#define Collective_hpp

#include "utilities.hpp"

class Collective :
    private NotCopyable
{
private:
    // Private constructor enforces singleton-like collectives,
    // where there are multiple collectives but each is a singleton of itself.
    Collective(const string& shortName, const string& fullName) :
        shortName(shortName), fullName(fullName) {}
    
public:
    // Returns the shortname (key) associated with the collective.
    const string& ShortName() const;
    
    // Returns the fullname (displayed to the user) of the collective.
    const string& FullName() const;
    
    // Creates a new collective and adds it to the set of known collectives.
    static void Create(string shortName, string fullName);
    
    // Returns the collective associated with the provided shortname.
    static const Collective& Get(const string& shortName);
    
    // Returns the iterable collection of all collectives.
    static const map<string, Collective>& GetAll();
    
    static string RandomShortName();
    static const Collective& Random();
    
private:
    const string shortName;
    const string fullName;
    
    static map<string, Collective> allCollectives;
};

#endif /* Collective_hpp */
