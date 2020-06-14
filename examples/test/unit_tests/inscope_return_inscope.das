struct Foo
    a : int

[sideeffects]
def buggy(skip:bool)
    var in scope a = new Foo
    if skip
        return
    var in scope b = new Foo

[export]
def test
    buggy(true)
    return true
