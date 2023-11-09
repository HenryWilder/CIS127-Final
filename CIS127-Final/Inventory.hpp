#ifndef Inventory_hpp
#define Inventory_hpp

#include "utilities.hpp"
#include "Enums.hpp"

class Inventory :
    private NotCopyable
{
public:
    // Returns true if the item exists in the inventory; otherwise false.
    bool Has(Item item) const;
    
    // Returns the quantity of the requested item that exist in the inventory.
    // If the item is not in the inventory, returns 0.
    int Count(Item item) const;
    
    // Returns a readonly reference to the inventory map.
    const map<Item, int>& GetAll() const;
    
    bool IsEmpty() const;
    
    // Increases the count of the item by
    void Add(Item item, int count = 1);
    
    // If failed to remove the item (e.g. item is missing or in insufficient quantity), returns -1.
    // When trying to remove a greater quantity of the item than is available, none is removed.
    // If successful, returns how many of that item remain.
    int TryRemove(Item item, int count = 1);
    
    // Removes the specified quantity of the item. If the item is in insufficient quantity, it is entirely removed.
    // Returns true if the removal would have been successful under normal circumstances,
    // returns false if the quantity of the item was insufficient for the removal (but all were removed anyway).
    bool ForceRemove(Item item, int count = 1);
    
    // Eliminates all instances of the provided item from the inventory no matter the current quantity.
    void RemoveAll(Item item);
    
    // Eliminates all items from the inventory.
    void RemoveAll();
    
    // Prints the inventory to the console
    void Print(ostream& stream) const;
    
    // Prompts the user with a list of the item keys
    Item Prompt(const string& prompt) const;

    void Init();
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
private:
    map<Item, int> items;
};

#endif /* Inventory_hpp */
