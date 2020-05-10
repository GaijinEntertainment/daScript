options log = true
options rtti = true

require ast

struct Foo
    a : int
    b : array<Foo?>

[export]
def test
    let td = typeinfo(ast_typedecl type<Foo>)
    print("td = {describe(td)}\n")
    return true
