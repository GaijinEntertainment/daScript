require daslib/templates_boost
require daslib/ast_boost

[export]
def main
    print("hello, world\n")
    let bus = "bus_variable_name"
    let car = "car_variable_name"
    var blk <- qmacro <|
        let t = $$(bus)
        let q = $$(car)
        print("{q+t}\n")
    print(describe(blk))


options log
