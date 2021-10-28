require math

def add(a,b)
    return a + b

def show(a)
    print("{a}\n")
    return a

[export]
def main
    13 |> add(2) |> show |> add(3) |> show