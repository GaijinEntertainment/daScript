require t_counter

typedef Counter = $counter(type<int>)


def take_any_counter(var t : $counter(type<auto(TT)>)) : TT {
    print("any counter type is {typeinfo typename(type<TT>)}\n")
    return t.value()
}

[export]
def main {
    var c = Counter()
    c.next()
    take_any_counter(c)
}

