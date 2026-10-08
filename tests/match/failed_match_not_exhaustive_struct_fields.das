// struct patterns are checked field by field - a field value no arm covers is missing
options gen2

require daslib/match

expect 50503

enum Color {
    red
    green
}

struct Light {
    color : Color
    on : bool
}

[export]
def test {
    let l = Light(color = Color.green, on = false)
    return match (l) {
        Light(color = Color.red) => true
        Light(color = Color.green, on = true) => false
    }
}
