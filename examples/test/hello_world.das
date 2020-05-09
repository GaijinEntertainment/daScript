options log = true
options rtti = true

require ast

struct Foo
    a : int
    b : array<Foo?>

[export]
def test
    let td = typeinfo(ast_typedecl type<Foo>)
    debug(td,"\td=")
    print("\ntd = {td}\n")

    let flags = td.structType.flags
    print("flags = {flags}\n")

    return true
