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
