require fio

class Foo
    def const GetValue()
        return 2

def Bar( foo: Foo const& )
    return foo->GetValue()

[export]
def main
    let foo = new Foo()
    Bar(*foo)

