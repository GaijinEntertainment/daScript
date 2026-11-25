options gen2

module test_compile_dag_base public

// a comment the reader of a compile that starts with it loaded must see

def compile_dag_base() => 1

var compile_dag_counter = 0

def compile_dag_bump(var counter : int&) {
    counter ++
}
