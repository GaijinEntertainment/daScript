expect 40214:3

options no_init

var a = 13

def foo
    print("hello, world!\n")
    return 1

var b = foo()   // 40214: [init] is disabled in the options or CodeOfPolicies

[init]          // 40214: [init] is disabled in the options or CodeOfPolicies
def bar
    print("hello, world\n")

[finalize]
def car         // 40214: [init] is disabled in the options or CodeOfPolicies
    print("goodbye, world\n")

[export]
def main
    print("hello, world! {a}\n")


