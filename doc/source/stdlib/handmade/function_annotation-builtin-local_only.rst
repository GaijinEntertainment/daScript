Indicates that function can only accept local `make` expressions, like [[make tuple]] and [[make structure]]::

    [local_only (arg)]
    def take_local(arg)
        print("arg={arg}\n")

    struct Foo
        bar : int

    [export]
    def test
        take_local(1)             // 40102: call annotated by local_only failed, expecting [[...]]
        take_local(Foo(bar=1))   // ok, structure are local
        take_local((1,2.0,"3"))  // ok, tuple are local
        return true
