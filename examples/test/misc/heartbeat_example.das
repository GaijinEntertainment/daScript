require daslib/heartbeat


def foo
    print("hello\n")
    for i in range(3)
        print("step {i}\n")
    var q = 1
    while q < 3
        print("q = {q}\n")
        q ++

[export]
def main
    set_heartbeat <| @
        print(".\n")
    foo()

options log
