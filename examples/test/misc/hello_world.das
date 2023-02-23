require daslib/lpipe

def add(a,b)
    return a + b

[export]
def main
    var i = add(1)
    lpipe(2)
    print("i = {i}\n")

