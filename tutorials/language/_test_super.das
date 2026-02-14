options gen2
class Base {
    name : string
    def Base(n : string) {
        name = n
    }
    def greet {
        print("Base greet: {name}\n")
    }
}
class Derived : Base {
    extra : int
    def Derived(n : string; e : int) {
        super("Derived:" + n)
        extra = e
    }
    def override greet {
        super.greet()
        print("  extra = {extra}\n")
    }
}

class Counter {
    static total : int = 0
    id : int
    def Counter {
        total += 1
        id = total
    }
    def static getTotal : int {
        return total
    }
    def static reset {
        total = 0
    }
}

[export]
def main {
    var d = new Derived("test", 42)
    d.greet()

    var c1 = new Counter()
    var c2 = new Counter()
    print("total via static method: {Counter`getTotal()}\n")
    Counter`reset()
    print("after reset: {Counter`getTotal()}\n")
}
