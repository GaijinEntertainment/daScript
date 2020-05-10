options log = true
options rtti = true

require ast

struct Foo
    a : int
    b : array<Foo?>

[export]
def test
    let td <- typeinfo(ast_typedecl type<Foo>)
    print("td = {describe(td)}\n")
    var x = 1
    var y = 2
    x = 3
    y = 4
    let ed <- typeinfo(ast_expression x+y)
    print("ed = {describe(ed)}\n")
    return true
