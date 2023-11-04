#ifndef Inventory_hpp
#define Inventory_hpp

#include "utilities.hpp"

class Inventory :
    private NotCopyable
{
public:
    // Returns true if the item exists in the inventory; otherwise false.
    bool Has(const string& key) const;
    
    // Returns the quantity of the requested item that exist in the inventory.
    // If the item is not in the inventory, returns 0.
    int Count(const string& key) const;
    
    // Returns a readonly reference to the inventory map.
    const map<string, int>& GetAll() const;
    
    bool IsEmpty() const;
    
    // Increases the count of the item by
    void Add(const string& key, int count = 1);
    
    // If failed to remove the item (e.g. item is missing or in insufficient quantity), returns -1.
    // When trying to remove a greater quantity of the item than is available, none is removed.
    // If successful, returns how many of that item remain.
    int TryRemove(const string& key, int count = 1);
    
    // Removes the specified quantity of the item. If the item is in insufficient quantity, it is entirely removed.
    // Returns true if the removal would have been successful under normal circumstances,
    // returns false if the quantity of the item was insufficient for the removal (but all were removed anyway).
    bool ForceRemove(const string& key, int count = 1);
    
    // Eliminates all instances of the provided item from the inventory no matter the current quantity.
    void RemoveAll(const string& key);
    
    // Eliminates all items from the inventory.
    void RemoveAll();
    
    // Prints the inventory to the console
    void Print() const;
    
    // Prompts the user with a list of the item keys
    string Prompt(const string& prompt) const;

    void Init();
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
private:
    map<string, int> items;
};

#endif /* Inventory_hpp */
