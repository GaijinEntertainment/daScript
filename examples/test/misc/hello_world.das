require t_counter

typedef Counter = counter<int>

def take_any_counter(var t : counter<auto(TT)>) {
    print("any counter type is {typeinfo typename(type<TT>)}\n")
    t.next()
}

[export]
def main {
    var c = Counter()
    c.next()
    take_any_counter(c)
    print("count = {c.value()}\n")
}

