require llvm/llvm_jit
require jit

[jit]
def add ( a, b : int )
    return a + b

[export]
def main
    verify(is_jit_function(@@add))
    var c = add(11,12)
    print("result = {c}\n")
