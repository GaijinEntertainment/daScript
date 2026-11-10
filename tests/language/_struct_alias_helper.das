options gen2

module _struct_alias_helper public

struct AliasedBox {
    x : int
}

enum AliasedHue {
    red
    green
}

typedef AliasedNum = int

struct SharedBox {
    v : int
}

def SharedBox(v : int) : SharedBox => SharedBox(v = v)
