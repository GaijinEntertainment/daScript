require rtti

struct A
    b : int

[export]
def main
    var a = [[A b=13]]
    a.b ++
    print("{a}\n")
    print("THIS: {this_context()}\n")
    breakpoint()

