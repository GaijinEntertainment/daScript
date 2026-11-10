//! assigning, moving or struct-initializing a method field of a
//! sealed class or a sealed method does not compile
options gen2
expect 30952:5

class sealed Greeter {
    def hello : string {
        return "hello"
    }
}

class Base {
    def name : string {
        return "base"
    }
}

class Fixed : Base {
    def sealed name : string {
        return "fixed"
    }
}

def other(var self : Greeter) : string {
    return "other"
}

def other_name(var self : Base) : string {
    return "other"
}

[export]
def write_sealed_methods {
    var g = new Greeter()
    g.hello = @@other
    g.hello <- @@other
    var f = new Fixed()
    f.name = @@other_name
    var made = new Greeter(hello = @@other)
    var fixed = new Fixed(name = @@other_name)
}
