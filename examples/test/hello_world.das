options debugger
require daslib/debug

require daslib/safe_addr

class A
    name : string
    def func()
        print("123")
    def bang
        func()

[export]
def main
    var a = new A()
    a->bang()
