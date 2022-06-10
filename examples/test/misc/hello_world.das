struct Foo
    a : string

def operator == ( a:Foo; b )
    print("{a} == {b}:{typeinfo(typename b)}??\n")
    return false

def operator ++ ( a )
    print("anything goes\n")

[export]
def main
    var f : Foo
    f == 10
    f == "10"
    f++

options log
