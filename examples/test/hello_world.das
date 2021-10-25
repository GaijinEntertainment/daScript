class Foo
    def const GetValue(x:int; a:int=5)
        print("GetValue({x},{a})\n")
        return 2 + a

def Bar( foo: Foo const& )
    return foo->GetValue(1)

[export]
def main
    let foo = new Foo()
    Bar(*foo)

