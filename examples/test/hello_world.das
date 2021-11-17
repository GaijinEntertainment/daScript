def iliketomoveitmoveit ( var a )
    return <- a

[export]
def main
    var a <- [{for x in range(10); x}]
    print("a = {a}\n")
    a <- a
    print("after a = {a}\n")
    a <- iliketomoveitmoveit(a)
    print("after aa = {a}\n")
