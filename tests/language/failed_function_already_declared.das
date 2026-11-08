//! two functions with the same name and signature in one module do
//! not compile
// duplicate function declaration
// verifies compiler rejects two functions with identical signatures
options gen2
expect 30702   // function_already_declared

def test {
    assert(false)
}

def test {
    assert(false)
}

