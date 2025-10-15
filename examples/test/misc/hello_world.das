require t_counter

[unused_argument(dummy)]
def take_counter(foo : $counter(type<auto(TT)>); dummy : TT = type<TT>)  {
    /// this one uses dummy variable to force TT to be deduced
    print("counter TT is {typeinfo typename(type<TT>)}\n")
}

def take_another_counter(foo : $counter(type<auto(TT)>)) : TT {
    // this one uses result to force TT to be deduced
    print("another counter TT is {typeinfo typename(type<TT>)}\n")
    return default<TT>
}

def take_int_counter(var foo : $counter(type<int>)) {
    foo.next()
    print("int counter value is {foo.value()}\n")
}

[export]
def main {
    var foo = default< $counter(type<int>) >
    take_int_counter(foo)
    take_counter(foo)
    take_another_counter(default< $counter(type<float>) >)
}
