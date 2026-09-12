options gen2
options indenting = 4


[export]
def greet(who : string) : string {
    return "hola {who}"
}

[export]
def bump(n : int) : int {
    return n + 1
}

def internal_seed() : int {
    return 41
}
