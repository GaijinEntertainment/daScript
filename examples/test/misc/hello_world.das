require t_counter

[unused_argument(dummy)]
def take_counter(foo : $counter(type<auto(TT)>); dummy : TT = type<TT>)  {
    print("TT is {typeinfo typename(type<TT>)}\n")
}

[export]
def main {
    var foo = default< $counter(type<int>) >
    print("foo = {foo}\n")
    take_counter(foo)
}







