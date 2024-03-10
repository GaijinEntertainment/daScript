// this is port from https://github.com/frol/completely-unscientific-benchmarks
// naive is almost direct kotlin implementation
// regular is almost direct C++-raw implementation

// options log_infer_passes = true
// options log = true
// options log_nodes = true

include ../config.das

options persistent_heap = true
options solid_context

require daslib/random
require testProfile

struct Node
    x,y:int
    left, right: Node?

var
    seed:int4 = int4(1,1,1,1)

[unsafe_deref,hint(alwaysinline,hot)]
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

[hint(alwaysinline,hot)]
def merge(var lower,equal,greater: Node?): Node?
    return merge(merge(lower, equal), greater)

[unsafe_deref,hint(alwaysinline,hot)]
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

[hint(alwaysinline,hot)]
def split(var orig: Node?; var lower:Node?&; var equal:Node?&; var greater:Node?&; value: int)
	var equalOrGreater : Node?
    split(orig, lower, equalOrGreater, value)
    split(equalOrGreater, equal, greater, value + 1)

[hint(alwaysinline,hot)]
def hasValue(var mRoot:Node? &; x: int): bool
	var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    let res = equal != null
    mRoot = merge(lower, equal, greater)
    return res

[hint(alwaysinline,hot)]
def insert(var mRoot:Node? &; x: int)
    var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    if equal == null
    	equal = new [[Node x=x, y=random_big_int(seed)]]
    mRoot = merge(lower, equal, greater)

[hint(alwaysinline,hot)]
def erase_raw(var mRoot:Node? &; x: int)
    var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    mRoot = merge(lower, greater)
    unsafe
        delete equal

def mainTreeNode
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
            if tree |> hasValue(cur)
                res ++
    unsafe
	    delete tree
    return res

[export,no_aot,no_jit]
def main
    profile(10, "tree")  <|
        mainTreeNode()
    if is_cpp_time()
        cpp_label()
        profile(10, "tree")  <|
            testProfile::testTree()
        run_files([[TestFile
            luauFile = "tree.luau",
            luaFile = "tree.lua",
            luaJitFile = "tree.lua",
            // nutFile = "tree.nut",
            // asFile = "tree.as",
            jsFile = "tree.js",
            monoFile = "tree.cs",
            dotNetFile = "tree.cs",
        ]])
