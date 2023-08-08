var foo : bool

[export]
def main
    print("hello!\n")
    if true
        var inscope a : array<int>
        if foo
            return
        var inscope b : array<int>
        print("{a} {b}\n")
finally
    print("bye\n")

options log
