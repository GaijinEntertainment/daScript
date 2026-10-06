options gen2
options indenting = 4


[export]
def greet(who : string) : string => "hola {who}"

[export]
def bump(n : int) : int => n + 1

def internal_seed() : int => 41
