options gen2

module _standalone_dep_call public

require daslib/fio

// A required module whose FUNCTIONS the entry module calls at run time - a plain one that reaches
// a C++ extern only from here, and a class method dispatched through its Func slot.
def dep_probe(x : int) : int {
    return x + length(getcwd())
}

class DepProbe {
    def method(x : int) : int {
        return x * 2
    }
}
