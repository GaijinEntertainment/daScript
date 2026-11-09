options gen2

require daslib/ast_boost

//! a child for the daslang host tests: leaves a gc_node on the
//! thread root at [init], so the parent reads the exit code a
//! GC COMPILE LEAK report carries past a main that returns 0
var leaked : TypeDeclPtr

[init]
def leak_at_init {
    leaked = new TypeDecl(baseType = Type.tInt)
}

[export]
def main() : int => 0
