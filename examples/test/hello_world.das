require daslib/templates

[template(a)]
def foo(a:auto(TT))
    unsafe
        return <- generator<TT&#> () <| $ ()
            return false

[export]
def main
    for x in foo(type<int>)
        print("x = {x}\n")

options log_infer_passes

