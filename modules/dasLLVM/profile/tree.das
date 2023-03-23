// this is port from https://github.com/frol/completely-unscientific-benchmarks
// naive is almost direct kotlin implementation
// regular is almost direct C++-raw implementation

// options log_infer_passes = true
// options log = true
// options log_nodes = true

options persistent_heap = true

require daslib/random

require llvm/llvm_jit
require jit

struct Node
    x,y:int
    left, right: Node?

var
    seed:int4 = int4(1,1,1,1)

[jit]
def private finalize ( var that:Node?& explicit -const )
    unsafe
	    if that != null
		    finalize(*that)
		    delete explicit that
		    that = null

[jit]
def private finalize ( var that:Node explicit )
    unsafe
	    finalize(that.left)
	    finalize(that.right)
	    memzero(that)

[jit,unsafe_deref,hint(hot)]
def merge(var lower, greater: Node?): Node?
    if lower == null
    	return greater
    if greater == null
    	return lower
    if lower.y < greater.y
        lower.right = merge(lower.right, greater)
        return lower
    else
        greater.left = merge(lower, greater.left)
        return greater

[jit,hint(alwaysinline,hot)]
def merge(var lower,equal,greater: Node?): Node?
    return merge(merge(lower, equal), greater)

[jit,unsafe_deref,hint(hot)]
def split(var orig:Node?; var lower:Node?&; var greaterOrEqual:Node?&; value: int)
    if orig == null
    	lower = null
		greaterOrEqual = null
    elif orig.x < value
		lower = orig
		split(lower.right,lower.right,greaterOrEqual,value)
    else
		greaterOrEqual = orig
		split(greaterOrEqual.left,lower,greaterOrEqual.left,value)

[jit,hint(alwaysinline,hot)]
def split(var orig: Node?; var lower:Node?&; var equal:Node?&; var greater:Node?&; value: int)
	var equalOrGreater : Node?
    split(orig, lower, equalOrGreater, value)
    split(equalOrGreater, equal, greater, value + 1)

[jit,hint(alwaysinline,hot)]
def hasValue(var mRoot:Node? &; x: int): bool
	var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    let res = equal != null
    mRoot = merge(lower, equal, greater)
    return res

[jit,hint(alwaysinline,hot)]
def random_int_jit ( var seed:int4& )
    //! random integer 0..32767 (LCG_RAND_MAX)
    seed.x = 214013*seed.x+2531011
    return (seed.x>>16) & LCG_RAND_MAX

[jit,hint(alwaysinline,hot)]
def insert(var mRoot:Node? &; x: int)
    var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    if equal == null
    	equal = new [[Node x=x, y=random_int_jit(seed)]]
    mRoot = merge(lower, equal, greater)

[jit,hint(alwaysinline,hot)]
def erase_raw(var mRoot:Node? &; x: int)
    var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    mRoot = merge(lower, greater)
    unsafe
        delete equal

[jit]
def testTreeJit
    var tree:Node?
    var cur = 5
    var res = 0
    for i in range(1, 1000000)
        let a = i % 3
        cur = (cur * 57 + 43) % 10007
        if a == 0
            tree |> insert(cur)
        elif a == 1
            tree |> erase_raw(cur)
        elif a == 2
            res += (tree |> hasValue(cur)) ? 1 : 0
    unsafe
	    delete tree
    return res

[export]
def main
    verify(is_jit_function(@@testTreeJit))
    profile(10, "tree jit")  <|
        testTreeJit()

