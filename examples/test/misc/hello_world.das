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
    var blk <- qmacro <|
        let t = $$(bus)
        let q = $$e([[auto a1,car]])
        print("{q}{t}\n")
    print(describe(blk))

options log
