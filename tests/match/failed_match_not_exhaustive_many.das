// more than three missing values are reported as three and more
options gen2

require daslib/match

expect 50503

enum Dir {
    north
    south
    east
    west
    up
}

[export]
def test {
    let d = Dir.up
    return match (d) {
        Dir.north => true
    }
}
