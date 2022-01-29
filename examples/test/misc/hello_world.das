
[export]
def main
    var a <- [{int 1;2;3;4;5}]
    var b <- a[1..3]
    var c : array<int>
    c |> push(a[0..5])
    c |> push([[int 6; 7]])
    print("a = {a} b = {b} c = {c}\n")

options log
