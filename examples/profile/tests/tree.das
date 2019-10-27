//this is port from https://github.com/frol/completely-unscientific-benchmarks
options heap = 200000//not required hint to increase initial heap footprint. <1% of performance. 200kb is enough

require random
require testProfile

struct Node
    x,y:int
    left, right: Node?

var
    seed:int4 = int4(1,1,1,1)

[unsafe, unsafe_deref]
def destroy(var root:Node?)
    if root != null
		destroy(root.left)
		destroy(root.right)
		delete root

[unsafe_deref]
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

def merge(var lower,equal,greater: Node?): Node?
    return merge(merge(lower, equal), greater)

[unsafe_deref]
def split(var orig:Node?; var lower:Node?&; var greaterOrEqual:Node?&; value: int)
    if orig == null
    	lower = null
		greaterOrEqual = null
		return
    if orig.x < value
		lower = orig
		split(lower.right,lower.right,greaterOrEqual,value)
    else
		greaterOrEqual = orig
		split(greaterOrEqual.left,lower,greaterOrEqual.left,value)

def split(var orig: Node?; var lower:Node?&; var equal:Node?&; var greater:Node?&; value: int)
	var equalOrGreater : Node?
    split(orig, lower, equalOrGreater, value)
    split(equalOrGreater, equal, greater, value + 1)

def hasValue(var mRoot:Node? &; x: int): bool
	var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    let res = equal != null
    mRoot = merge(lower, equal, greater)
    return res

def insert(var mRoot:Node? &; x: int)
    var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    if equal == null
    	equal = new [[Node x=x, y=random_int(seed)]]
    mRoot = merge(lower, equal, greater)

def erase_raw(var mRoot:Node? &; x: int)
    var lower, equal, greater : Node?
    split(mRoot, lower, equal, greater, x)
    mRoot = merge(lower, greater)
    destroy(equal)

def main
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
	destroy(tree)
    //print("{res}")

[export]
def test
    profile(10, "tree")  <|
        main()
    profile(10, "tree C++")  <|
        testProfile::testTree()
	// print("total = {int(heap_bytes_allocated())}\n")
    // print("depth = {heap_depth()}\n")
    verify(heap_depth()==1)
    return true
