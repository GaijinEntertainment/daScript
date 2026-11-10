options gen2

module _struct_alias_twin public

struct SharedBox {
    w : int
}

def SharedBox(v : int) : SharedBox => SharedBox(w = v * 10)
