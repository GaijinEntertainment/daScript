// an arm can't declare a name its pattern binds - a let, a for iterator, a block argument or a comprehension variable shadows the binding, as outside a match
options gen2

require daslib/match

expect 30704, 30708:2, 30701

[export]
def test {
    let x = 3
    match (x) {
        n => $ {
            let n = 5
        }
    }
    match (x) {
        n => $ {
            for (n in range(2)) {
                print("{n}\n")
            }
        }
    }
    match (x) {
        n => $ {
            invoke($(n : int) { print("{n}\n") }, 10)
        }
    }
    let r = match (x) {
        k => [for (k in range(2)); k * 100]
    }
    return length(r)
}
