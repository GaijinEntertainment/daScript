// a statement match over a finite type must cover every value or end with _
options gen2

require daslib/match

expect 50503

enum Dir {
    north
    south
    east
}

[export]
def test {
    let d = Dir.east
    match (d) {
        Dir.north => $ { return false }
        Dir.south => $ { return false }
    }
    return true
}
