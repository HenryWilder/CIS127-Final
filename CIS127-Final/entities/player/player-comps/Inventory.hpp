#pragma once
#include "IPlayerComponent.hpp"
#include "helpers/enums.hpp"

class Inventory :
    public IPlayerComponent
{
public:
    // Returns true if the item exists in the inventory; otherwise false.
    bool HasItem(Item item) const;

    // Returns the quantity of the requested item that exist in the inventory.
    // If the item is not in the inventory, returns 0.
    int CountItem(Item item) const;

    // Returns a readonly reference to the inventory map.
    const map<Item, int> &GetAllItems( ) const
    {
        return items;
    }

    // Whether the inventory contains no items.
    bool IsInventoryEmpty( ) const
    {
        return items.empty( );
    }

    // Increases the count of the item by count.
    void AddItem(Item item, _In_range_(> , 0) int count = 1);

    // If failed to remove the item (e.g. item is missing or in insufficient quantity), returns -1.
    // When trying to remove a greater quantity of the item than is available, none is removed.
    // If successful, returns how many of that item remain.
    int TryRemoveItemQty(Item item, _In_range_(> , 0) int count = 1);

    // Removes the specified quantity of the item. If the item is in insufficient quantity, it is entirely removed.
    // Returns true if the removal would have been successful under normal circumstances,
    // returns false if the quantity of the item was insufficient for the removal (but all were removed anyway).
    bool ForceRemoveItemQty(Item item, _In_range_(> , 0) int count = 1);

    // Eliminates all instances of the provided item from the inventory no matter the current quantity.
    void RemoveAllOfItem(Item item)
    {
        items.erase(item);
    }

    // Eliminates all items from the inventory.
    void ClearInventory( )
    {
        items.clear( );
    }

    // Prints the inventory to the console
    void PrintInventory(ostream &stream) const;

    // Prompts the user with a list of the item keys
    Item PromptItem(const string &prompt) const;

    void Init( ) override;
    void Save(ostream &ofs) const override;
    void Load(istream &ifs) override;

private:
    map<Item, int> items;
};
