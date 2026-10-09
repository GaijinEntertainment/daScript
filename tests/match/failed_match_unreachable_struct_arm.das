// an arm whose struct values earlier struct arms already cover is unreachable
options gen2

require daslib/match

expect 50503

struct Light {
    level : int
    on : bool
}

[export]
def test {
    let l = Light(level = 1, on = false)
    return match (l) {
        Light(on = true) => true
        Light(on = false) => false
        Light(level = 3) => false
    }
}
