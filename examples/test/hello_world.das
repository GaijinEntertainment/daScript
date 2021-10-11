require daslib/lint_everything
require daslib/json_boost

def testReturn(a: int)
    if a > 0
        print("before return\n")
        return
        print("after return\n")

def testPanic(a: int)
    if a > 0
        print("before panic\n")
        panic("panic")
        print("after panic\n")

[export]
def main
    pass
    // testReturn(10)
    // testPanic(-10)

