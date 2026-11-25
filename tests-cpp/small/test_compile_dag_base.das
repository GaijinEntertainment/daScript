options gen2

module test_compile_dag_base public

// a comment a reader loaded before the compile must see

def compile_dag_base() => 1

var compile_dag_counter = 0

let compile_dag_limit = 1000

def compile_dag_bump(var counter : int&) {
    counter ++
}
