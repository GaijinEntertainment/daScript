// options log_infer_passes=true

require aaa

[export]
def dummy(var a,b:A)
    a := b

[export]
def dummy(var a:array<A>;var b:array<A>)
    a := b
