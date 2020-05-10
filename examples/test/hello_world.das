options log = true
options rtti = true

require ast

struct Foo
    a : int
    b : array<Foo?>

def add(a,b:int)
    return a + b

[export]
def test
    // ast_typedecl
    let td <- typeinfo(ast_typedecl type<Foo>)
    print("td = {describe(td)}\n")

    // ast_expression
    var x = 1
    var y = 2
    x = 3
    y = 4
    let ed <- typeinfo(ast_expression x+y)
    print("ed = {describe(ed)}\n")

    // ast_function
    let fd <- typeinfo(ast_function @@add)
    print("fd = {describe(fd)}\n")

    return true
