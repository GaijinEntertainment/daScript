options log = true
options log_nodes = true

[persistent]
class Foo
    count : int = 0
    def Foo ( ic : int )
        count = ic
    def update
        return count++

[export,unsafe]
def test
    var pFoo = new Foo(13)
    print("{pFoo->update()}\n")
    print("{pFoo->update()}\n")
    print("{pFoo->update()}\n")
    delete pFoo
    return true
