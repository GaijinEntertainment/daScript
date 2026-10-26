// a `?:` with a temporary arm is the temporary whenever the condition
// picks that arm, so a local reference bound to it would dangle
options gen2
expect 31019 : 2

struct Bag {
    xs : array<int>
}

def private make_bag : Bag {
    var b : Bag
    b.xs |> push(7)
    return <- b
}

def private select_bag(pick : bool) {
    var held : Bag
    let leftArm & = pick ? make_bag() : held
    let rightArm & = pick ? held : make_bag()
}

[export]
def main {
    select_bag(true)
}
