// a name an arm binds is a read-only copy of the matched part - assigning to it is an error
options gen2

require daslib/match

expect 30952

[export]
def test {
    var x = 1
    match (x) {
        n => $ { n = 2 }
    }
    return x
}
