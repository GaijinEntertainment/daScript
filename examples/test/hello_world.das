options log = true
options log_infer_passes = true

struct Foo
    a : int

[export]
def test
    let aC : Foo
    var aV : Foo
    let t : int

    if aC is Foo
        print("aC is Foo\n")
    if aV is Foo
        print("aV is Foo\n")
    if t is Foo
        print("t is Foo\n")
    return true

