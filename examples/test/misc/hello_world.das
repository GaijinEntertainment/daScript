require daslib/constant_expression

[generic, constant_expression(constString)]
def take_const(constString:string)
    print("take const {constString}\n")
    static_if !typeinfo(is_argument constString)
        static_if constString=="one"
            return 1
        static_elif constString=="two"
            return 2.0
        else
            return constString

[export]
def main
    take_const("one")
    take_const("two")
    take_const("three")

options log
