options gen2

require daslib/match

typedef Name = string

[export]
def test(v : variant<i : int; s : Name>) : int {
    return match (v) {
        i(n) => n
    }
}
