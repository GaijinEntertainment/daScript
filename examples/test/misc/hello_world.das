require daslib/constant_expression

[generic, constant_expression(constString=false)]
def take_const(constString:string)
    static_if !typeinfo(is_argument constString)
        print("take const {constString}\n")
        static_if constString=="one"
            return 1
        static_elif constString=="two"
            return 2.0
        else
            return constString
    else
        print("take value {constString}\n")

[export]
def main
    take_const("one")
    take_const("two")
    take_const("three")

    var qq = "blah"
    qq += "_blah"
    take_const(qq)

options log
