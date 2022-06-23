require daslib/templates_boost
require daslib/ast_boost
require daslib/strings

[export]
def main
    print("hello, world\n")
    let bus = "bus_variable_name"
    let car = "car_variable_name"
    var a1 = 1
    var a2 = a1 + 1
    var a3 = a1 + 13
    var expr <- quote(length(bus))
    var qqblk : array<ExpressionPtr>
    for i in range(3)
        qqblk |> emplace <| qmacro(print("{$$v(i)}\n"))
    var blk <- qmacro <|
        let t = $$i(bus)
        let q = $$v([[auto a1,car]])
        let lb = $$(expr)
        print("{q}{t}{lb}\n")
        $$b(qqblk)
    print(describe(blk))


