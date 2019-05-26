// options logInferPasses = true

struct Foo {}

[export]
def test:bool
    print("sizeof Foo is {typeinfo(sizeof type Foo)}\n")
    return true

