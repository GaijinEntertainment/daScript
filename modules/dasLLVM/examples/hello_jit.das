require llvm/llvm_jit

[jit_llvm]
def add ( a, b : int )
    return a + b

[export]
def main
    var c = add(11,12)
    print("result = {c}\n")

