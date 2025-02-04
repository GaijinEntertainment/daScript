/*
    this was crashing due to shadowing check on a block outside of function
*/

struct EquipSlot
	listSize : int
	itemTypes : array<string>
	items : array<string>
	isPrimary : bool
	atLeastOne : string
	ingameWeaponSlot : string
	uiOrder : int

let shared
	squadScheme <- {"curVehicle" => EquipSlot(uninitialized itemTypes := array("vehicle"))}

[export]
def test
	return true
