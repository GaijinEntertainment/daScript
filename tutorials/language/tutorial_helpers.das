options gen2
module tutorial_helpers public

struct Item {
    name : string
    value : int
}

def make_item(n : string; v : int) : Item => Item(name = n, value = v)

def describe(item : Item) : string => "{item.name} (worth {item.value})"

def private internal_id(item : Item) : int => item.value * 31

let MAX_ITEMS = 100
